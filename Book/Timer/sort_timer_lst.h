#ifndef LST_TIMER
#define LST_TIMER

#include <time.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 64

class util_timer; //前向声明

struct client_data {
    sockaddr_in address;
    int sockfd;
    char buf[BUFFER_SIZE];
    util_timer *timer;
    int index;
};

//定时器类
class util_timer {
public:
    util_timer():prev(NULL),next(NULL) {}

public:
    time_t expire; //任务超时时间
    void (*cb_func)(client_data *); //任务回调函数
    //回调函数处理的用户数据，由定时器的执行者传递给回调函数
    client_data *user_data;
    util_timer *prev;//指向上一个定时器
    util_timer *next;//指向下一个定时器
};

//定时器链表，它是一个升序、双向链表，且带有头节点和尾结点
class sort_timer_lst {
public:
    sort_timer_lst() : head(NULL), tail(NULL) {}
    //链表被销毁时，删除其中所有定时器
    ~sort_timer_lst() {
        util_timer *tmp = head;
        while(tmp) {
            head = tmp->next;
            delete tmp;
            tmp = tmp->next;
        }
    }
    //将目标定时器timer添加到链表中
    void add_timer(util_timer *timer) {
        if(!timer) {
            return;
        }
        if(!head) {
            head = tail = timer;
            return;
        }
        if(timer->expire < head->expire) {
            timer->next = head;
            head->prev = timer;
            head = timer;
            return;
        }
        add_timer(timer, head); //这是add_timer重载的一种方式，实现指定位置插入
    }
    //当某个定时任务发生变化时，调整对应定时器在链表中的位置，当前只考虑被调整的
    //定时器超时时间延长的情况
    void adjust_timer(util_timer* timer) {
        if(!timer) {
            return;
        }
        util_timer *tmp = timer->next;
        if(!tmp || (timer->expire < tmp->expire)) {
            return;
        }
        //如果是链表的头节点，取出重新插入。
        if(timer == head) {
            head = head->next;
            head->prev = NULL;
            timer->next = NULL;
            add_timer(timer, head);
        }
        //如果是中间结点，同样取出重新插入
        else {
            timer->prev->next = timer->next;
            timer->next->prev = timer->prev;
            add_timer(timer, timer->next);
        }
    }
    //将目标定时器timer从链表中删除
    void del_timer(util_timer* timer) {
        if(!timer) {
            return;
        }
        //下边整个条件成立表示链表中只有一个定时器
        if((timer == head) && (timer == tail)) {
            delete timer;
            head = NULL;
            tail = NULL;
            return;
        }
        //走到这里说明链表中至少有两个结点，如果删除的是头节点
        if(timer == head) {
            head = head->next;
            head->prev = NULL;
            delete timer;
            return;
        }
        //如果是尾结点
        if(timer == tail) {
            tail = timer->prev;
            timer->prev->next = NULL;
            delete timer;
            return;
        }
        //如果位于中间
        timer->prev->next = timer->next;
        timer->next->prev = timer->prev;
        delete timer;
    }
    //SIGALRM信号每次被触发就在其信号处理函数(如果是统一事件源则在主函数处理)中执行一次tick函数，以处理链表上到期的任务。
    void tick() {
        if(!head) {
            return;
        }
        printf("检查定时器链表(每20秒检查一次)\n");
        time_t cur = time(NULL);//获取当前时间
        util_timer *tmp = head;
        while(tmp) {
            if(tmp->expire > cur) {
                break;
            }
            //调用定时器的回调函数，执行定时任务
            tmp->cb_func(tmp->user_data);
            //执行完定时器中的定时任务之后，就将他删除，并重置头节点
            head = tmp->next;
            if(head) {
                head->prev = NULL;
            }
            delete tmp;
            tmp = head;
        }
    }
private:
    //一个重载的辅助函数
    void add_timer(util_timer* timer, util_timer * lst_head) {
        util_timer *prev = lst_head;
        util_timer *tmp = prev->next;
        while(tmp) {
            if(tmp->expire > timer->expire) {
                timer->next = tmp;
                timer->prev = tmp->prev;
                tmp->prev->next = timer;
                tmp->prev = timer;
                break;
            }
            prev = tmp;
            tmp = tmp->next; 
        }
        //如果遍历完链表还没找到合适的，则插到链表尾部
        if(!tmp) {
            prev->next = timer;
            timer->prev = prev;
            timer->next = NULL;
            tail = timer;
        }
    }
private:
    util_timer *head;
    util_timer *tail;
};
#endif