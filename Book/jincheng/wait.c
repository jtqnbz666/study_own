#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
	
	if(fork())
	{
		printf("子进程被创建了！\n");
		wait(0);//需要在子进程被创建后再使用！作用是等待子进程结束回收资源
	}
	else
	{
		for(int i=0;i<5;i++)
		{
			printf("子进程：%d\n",i);
			sleep(1);
		}
	}  

	printf("父进程结束！！\n");
}
