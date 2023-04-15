#nclude<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

int n=0;
//定义变量
pthread_spinlock_t  spin;

void *fun(void *a)
{
	for(int i=0;i<5000000;i++)
	{
		//上锁
		pthread_spin_lock(&spin);
		n++;
		//解锁
		pthread_spin_unlock(&spin);
	}
}
int main()
{
	pthread_t p1,p2;
	//初始化自旋锁
	pthread_spin_init(&spin,PTHREAD_PROCESS_PRIVATE);

	pthread_create(&p1,NULL,fun,NULL);
	pthread_create(&p2,NULL,fun,NULL);

	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	printf("n:%d\n",n);//10000000
	//销毁锁
	pthread_spin_destroy(&spin);
	return 0;


}
