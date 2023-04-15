#include<stdio.h>
#include<pthread.h>

struct Student 
{
char name[20];
int age;
float score;
};

int num=0;
void*pFunc(void *arg)
{
	int m=0;
	while(1)
	{
		if(m==2)pthread_exit();		//自己结束
	printf("线程一--%d\n",m++);
	printf("num:%d\n",num++);
	printf("线程一max：%d\n",*(int *)arg);
	sleep(1);
	}
}
void*pFunc2(void *arg)
{
	int m=0;
	struct Student* p=(struct Student*)arg;
	while(1)
	{
	printf("线程二--%d\n",m++);
	printf("num:%d\n",num++);
	//printf("线程二max：%d\n",*(int *)arg);
	printf("name:%s ,age:%d ,score:%g\n",p->name,p->age,p->score);
	sleep(1);
	}
}
int main()
{
	int n=0;
	int max=666;
	struct Student s={"江涛",20,66.66};
	pthread_t pid,pid1;
	pthread_create(&pid,NULL,pFunc,&max);
	pthread_create(&pid1,NULL,pFunc2,(void*)&s);
	//while(1)
	
		for(int i=0;i<5;i++)
		{
			printf("主函数--%d\n",n++);
			printf("num:%d\n",num++);
			if(i==1)pthread_cancel(pid1); 	/结束别的线程
			sleep(1);
		}
	
	printf("主线程结束！\n");
	return 0;
}