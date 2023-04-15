#include "spinlock.h"
#include "timewheel.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#if defined(__APPLE__)
#include <AvailabilityMacros.h>
#include <sys/time.h>
#include <mach/task.h>
#include <mach/mach.h>
#else
#include <time.h>
#endif
//这个时间轮是skynet中的设计， 时间精度是10ms

//time是一直++的，秒针的0位置有用，分针时针的0 位置无用，超过范围就会报错，所以设计时要注意
//比如在3秒添加一个70s的任务，那么就用当前时间time0(3) + msc(70) = expire(73), 
//就会以73来存储，

// 秒针 n % 60
// 分针 (n/60) % 60
// 时针 (n/3600) % 12

//3种驱动方式 : 1.epoll_wait, 2.usleep(多线程可能用到) 3. timerfd(linux的机制)
typedef struct link_list {		//这里是直接用这个链结构来管理一条任务链表，而高性能书上直接用的任务结点指针，没有使用这种管理的方式
	timer_node_t head;
	timer_node_t *tail;
}link_list_t;

typedef struct timer {
	link_list_t near[TIME_NEAR];
	link_list_t t[4][TIME_LEVEL];
	struct spinlock lock;
	uint32_t time;		//记录当前时间轮转动到哪个位置，一直在++
	uint64_t current;
	uint64_t current_point;		//每次检查过期定时器时，将这个时间更新为最新系统时间，大于等于第四个参数time，197行
}s_timer_t;

static s_timer_t * TI = NULL;


//list->head是辅助头节点
timer_node_t *
link_clear(link_list_t *list) {	//清除这条链，
	timer_node_t * ret = list->head.next;
	list->head.next = 0;
	list->tail = &(list->head);

	return ret;
}

void
link(link_list_t *list, timer_node_t *node) {		//添加定时任务结点到具体的一条链表上
	list->tail->next = node;
	list->tail = node;
	node->next=0;
}


// 秒针 n % 60
// 分针 (n/60) % 60
// 时针 (n/3600) % 12
void
add_node(s_timer_t *T, timer_node_t *node) {		//添加一个定时任务到定时器类中，具体实现还是靠link（），同时也是更新定时器位置的关键函数
	uint32_t time=node->expire;		//73
	uint32_t current_time=T->time;		//3
	uint32_t msec = time - current_time;	//70
	// 细节就是，第一层的0位置是有用的，后边的四层的0位置都是无用的，以256带入为例。  
	/*   
	0 1 2 ... 255
	0    1    2    3 	
        256  512  768

	*/
	if (msec < TIME_NEAR) { //[0, 0x100)
		link(&T->near[time&TIME_NEAR_MASK],node);
	} else if (msec < (1 << (TIME_NEAR_SHIFT+TIME_LEVEL_SHIFT))) {//[0x100, 0x4000)
		link(&T->t[0][((time>>TIME_NEAR_SHIFT) & TIME_LEVEL_MASK)],node);	
	} else if (msec < (1 << (TIME_NEAR_SHIFT+2*TIME_LEVEL_SHIFT))) {//[0x4000, 0x100000)
		link(&T->t[1][((time>>(TIME_NEAR_SHIFT + TIME_LEVEL_SHIFT)) & TIME_LEVEL_MASK)],node);	
	} else if (msec < (1 << (TIME_NEAR_SHIFT+3*TIME_LEVEL_SHIFT))) {//[0x100000, 0x4000000)
		link(&T->t[2][((time>>(TIME_NEAR_SHIFT + 2*TIME_LEVEL_SHIFT)) & TIME_LEVEL_MASK)],node);	
	} else {//[0x4000000, 0xffffffff]
		link(&T->t[3][((time>>(TIME_NEAR_SHIFT + 3*TIME_LEVEL_SHIFT)) & TIME_LEVEL_MASK)],node);	
	}
}

timer_node_t*
add_timer(int time, handler_pt func, int threadid) {	//添加一个定时任务
	timer_node_t *node = (timer_node_t *)malloc(sizeof(*node));
	spinlock_lock(&TI->lock);

//比如在3秒添加一个70s的任务，那么就用当前时间time0(3) + msc(70) = expire(73), 
	node->expire = time+TI->time;// 每10ms加10，
	node->callback = func;
	node->id = threadid;
	if (time <= 0) {
		node->callback(node);
		free(node);
		spinlock_unlock(&TI->lock);
		return NULL;
	}
	add_node(TI, node);
	spinlock_unlock(&TI->lock);
	return node;
}

void
move_list(s_timer_t *T, int level, int idx) {		//更新一条任务链的位置
	timer_node_t *current = link_clear(&T->t[level][idx]);
	while (current) {
		timer_node_t *temp=current->next;
		add_node(T,current);			// 重中之重来了， 其实更新位置就是把节点拿出来，重新加入
		current=temp;
	}
}

void 
timer_update(s_timer_t *T) {	//定时器更新
	spinlock_lock(&T->lock);
	timer_execute(T); //执行  
	timer_shift(T);			 //每次 将 T.time++;
	timer_execute(T);
	spinlock_unlock(&T->lock);
}

//对于这个函数，自己把256这个数带进去看一下过程
	/*   
	// 假设我们举例第二层及之后全是4个大小，那么如下
	0 1 2 ... 255			第一层
	0    1    2    3 		第二层
        256  512  768  1024		这里的1024已经不是这一层能处理的了，所以需要去下一层找
    0    1    2    3    	第三层
		1024 2048 3072 4096
	*/
void
timer_shift(s_timer_t *T) {			//时间轮更新，下一层往上一层传递
	int mask = TIME_NEAR; //256
	uint32_t ct = ++T->time;	//时间轮的转动
	if (ct == 0) { 
		//为0 则说明超出数字能表示的最大范围了
		move_list(T, 3, 0);
	} else {
		// ct / 256
		uint32_t time = ct >> TIME_NEAR_SHIFT;//8
		int i=0;
		// ct % 256 == 0
		// 以第一次进入为例
		while ((ct & (mask-1))==0) {			//刚开始ct是256 ，满足条件于是进入
			int idx=time & TIME_LEVEL_MASK;//63     那么这里就是1
			if (idx!=0) {	
				move_list(T, i, idx);				//每次移动都是直接移动到第一层，而不是上一层
				break;				
			}
			mask <<= TIME_LEVEL_SHIFT;//6
			time >>= TIME_LEVEL_SHIFT;
			++i;
		}
	}
}

void
dispatch_list(timer_node_t *current) {		//定时任务的执行
	do {
		timer_node_t * temp = current;
		current=current->next;
        if (temp->cancel == 0)
            temp->callback(temp);
		free(temp);
	} while (current);
}

void
timer_execute(s_timer_t *T) {		//定时检查定时器状态
	int idx = T->time & TIME_NEAR_MASK; //一直处理的任务都是第一层级上的
	
	while (T->near[idx].head.next) {
		timer_node_t *current = link_clear(&T->near[idx]);
		spinlock_unlock(&T->lock);
		dispatch_list(current);
		spinlock_lock(&T->lock);
	}
}


void
del_timer(timer_node_t *node) {
    node->cancel = 1;
}

s_timer_t *
timer_create_timer() {
	s_timer_t *r=(s_timer_t *)malloc(sizeof(s_timer_t));
	memset(r,0,sizeof(*r));
	int i,j;
	for (i=0;i<TIME_NEAR;i++) {
		link_clear(&r->near[i]);
	}
	for (i=0;i<4;i++) {
		for (j=0;j<TIME_LEVEL;j++) {
			link_clear(&r->t[i][j]);
		}
	}
	spinlock_init(&r->lock);
	r->current = 0;
	return r;
}

uint64_t
gettime() {
	uint64_t t;
#if !defined(__APPLE__) || defined(AVAILABLE_MAC_OS_X_VERSION_10_12_AND_LATER)
	struct timespec ti;
	clock_gettime(CLOCK_MONOTONIC, &ti);
	t = (uint64_t)ti.tv_sec * 100;
	t += ti.tv_nsec / 10000000;
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	t = (uint64_t)tv.tv_sec * 100;
	t += tv.tv_usec / 10000;
#endif
	return t;
}

void
expire_timer(void) {		//相当于tick函数，定时检查定时器列表
	uint64_t cp = gettime();
	if (cp != TI->current_point) {
		uint32_t diff = (uint32_t)(cp - TI->current_point);
		TI->current_point = cp;
		int i;
		for (i=0; i<diff; i++) {
			timer_update(TI);
		}
	}
}

void 
init_timer(void) {
	TI = timer_create_timer();
	TI->current_point = gettime();
}

void
clear_timer() {
	int i,j;
	for (i=0;i<TIME_NEAR;i++) {
		link_list_t * list = &TI->near[i];
		timer_node_t* current = list->head.next;
		while(current) {
			timer_node_t * temp = current;
			current = current->next;
			free(temp);
		}
		link_clear(&TI->near[i]);
	}
	for (i=0;i<4;i++) {
		for (j=0;j<TIME_LEVEL;j++) {
			link_list_t * list = &TI->t[i][j];
			timer_node_t* current = list->head.next;
			while (current) {
				timer_node_t * temp = current;
				current = current->next;
				free(temp);
			}
			link_clear(&TI->t[i][j]);
		}
	}
}
