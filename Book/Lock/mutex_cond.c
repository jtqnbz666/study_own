#include <stdio.h>
#include<pthread.h>
pthread_mutex_t mutex;
pthread_cond_t condc,condp;
int buffer=0;//生产者消费者使用的缓冲区
#define Max 10

void*producer(void*ptr)
{
	for(int i=1;i<Max;i++)
	{
		pthread_mutex_lock(&mutex);
		if(buffer!=0)pthread_cond_wait(&condp,&mutex);
		buffer=i;
		printf("我生产了：%d\n",buffer);
		pthread_cond_signal(&condc);
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(0);
}
void*consumer(void *ptr)
{
	for(int i=1;i<Max;i++)
	{
		pthread_mutex_lock(&mutex);
		if(buffer==0)pthread_cond_wait(&condc,&mutex);
		printf("我消费了:%d\n",buffer);
		buffer=0;
		pthread_cond_signal(&condp);
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(0);
}

int main()
{
	pthread_mutex_init(&mutex,0);
	pthread_cond_init(&condp,0);
	pthread_cond_init(&condc,0);
	pthread_t pro,con;//创建两个线程句柄
	pthread_create(&pro,NULL,producer,NULL);
	pthread_create(&con,NULL,consumer,NULL);
	pthread_join(pro,0);
	pthread_join(con,0);
		pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&condp);
	pthread_cond_destroy(&condc);
	return 0;
}