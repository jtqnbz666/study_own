#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int n=0;
void *fun (void*a)
{
	for(int i=0;i<500000000;i++)
	{
		n++;
	}
}

int main()
{
	pthread_t p1,p2;
	pthread_create(&p1,NULL,fun,NULL);
	pthread_create(&p2,NULL,fun,NULL);
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	print("n:%d\n",n);//10000000000


	return 0;
}