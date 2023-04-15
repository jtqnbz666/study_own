#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include <sys/mman.h>

#define THREAD_SIZE     10

int count = 0;
pthread_mutex_t mutex;
pthread_spinlock_t spinlock;
pthread_rwlock_t rwlock;

// add $1 %ex;  at&t        Unix和Linux
// add ex, 1;  intel        windows和Dos

int inc(int *value, int add) {

	int old;

	__asm__ volatile ( //有了volatile的修饰，就能够保证，每次都是去内存取最新数据，而不是寄存器中取
			"lock; xaddl %2, %1;" // "lock; xchg %2, %1, %3;" 
			: "=a" (old)
			: "m" (*value), "a" (add)
			: "cc", "memory"
			);

	return old;
}


// 
void *func(void *arg) {

	int *pcount = (int *)arg;

	int i = 0;
	while (i++ < 100000) {
#if 0
		(*pcount) ++;
#elif 0

		pthread_mutex_lock(&mutex);
		(*pcount) ++;
		pthread_mutex_unlock(&mutex);

#elif 0

		if (0 != pthread_mutex_trylock(&mutex)) {
			i --;
			continue;
		}
		(*pcount) ++;
		pthread_mutex_unlock(&mutex);
#elif 0

		pthread_spin_lock(&spinlock);
		(*pcount) ++;
		pthread_spin_unlock(&spinlock);
#elif 0

		pthread_rwlock_wrlock(&rwlock);
		(*pcount) ++;
		pthread_rwlock_unlock(&rwlock);

#else

		inc(pcount, 1);

#endif
		usleep(1);
	}

}


int main() {
#if 0
	pthread_t threadid[THREAD_SIZE] = {0};

	pthread_mutex_init(&mutex, NULL);
	pthread_spin_init(&spinlock, PTHREAD_PROCESS_SHARED);
	pthread_rwlock_init(&rwlock, NULL);

	int i = 0;
	int count = 0;
	for (i = 0;i < THREAD_SIZE;i ++) {
		int ret = pthread_create(&threadid[i], NULL, func, &count);
		if (ret) {
			break;
		}
	}

	for (i = 0;i < 100;i ++) {
		pthread_rwlock_rdlock(&rwlock);
		printf("count --> %d\n", count);
		pthread_rwlock_unlock(&rwlock);

		sleep(1);
	}
#else

	int *pcount = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANON|MAP_SHARED, -1, 0);


	int i = 0;
	pid_t pid = 0;
	for (i = 0;i < THREAD_SIZE;i ++) {

		pid = fork();
		if (pid <= 0) {
			usleep(1);
			break;
		}
	}


	if (pid > 0) { // 

		for (i = 0;i < 100;i ++) {
			printf("count --> %d\n",  (*pcount));
			sleep(1);
		}

	} else {

		int i = 0;
		while (i++ < 100000)  {
#if 0            
			(*pcount) ++;
#else
			inc(pcount, 1);
#endif
			usleep(1);
		}

	}




#endif 
}

