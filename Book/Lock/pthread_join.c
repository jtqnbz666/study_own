#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int n=555,m=666;

void *f1(void *arg)
{
	for(int i=0;i<10;i++)
	{
		printf("f1-------%d\n",i);
		sleep(1);
	}
	return (void*)&n;
}

void* f2(void*arg)
{
	for(int i=0;i<8;i++)
	{
		printf("f2-------%d\n",i);
		sleep(1);
	}
	return (void*)&n;
}

int main()
{
	int *ret1=malloc(4);
	int *ret2=malloc(4);
	*ret1=10;
	*ret2=20;
	printf("ret1:%d\n",*ret1);
	printf("ret2:%d\n",*ret2);
	pthread_t p1,p2;
	pthread_create(&p1,NULL,f1,NULL);
	pthread_create(&p2,NULL,f2,NULL);
	for(int i=0;i<5;i++)
	{
		printf("main==========%d\n",i);
		sleep(1);
	}
	pthread_join(p1,(void**)&ret1);
	printf("ret1:%d\n",*ret1);
	pthread_join(p2,(void**)&ret2);
	printf("ret2:%d\n",*ret2);
	return 0;
}