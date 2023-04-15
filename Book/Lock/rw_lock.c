#include<stdio.h>
#include<pthread.h>
//读读相容  ，读写相斥   写写相斥

pthread_rwlock_t rwlock;
int n=0;
void *fun1 (void*a)
{
	for(int i=0;i<500;i++)
	{
		pthread_rwlock_wrlock(&rwlock);//写锁
		//pthread_rwlock_rdlock(&rwlock);//读锁
		n++;
		pthread_rwlock_unlock(&rwlock);
	}
}
void *fun2 (void*a)
{
	for(int i=0;i<500;i++)
	{
		pthread_rwlock_wrlock(&rwlock);
		n++;
		pthread_rwlock_unlock(&rwlock);
	}
}

int main()
{
	pthread_t p1,p2;

	//1.初始化读写锁 
	pthread_rwlock_init(&rwlock,0);
	pthread_create(&p1,NULL,fun1,NULL);
	pthread_create(&p2,NULL,fun2,NULL);
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	printf("n:%d\n",n);//1000
	pthread_rwlock_destroy(&rwlock);

	return 0;
}
