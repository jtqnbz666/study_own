#include <stdio.h>
#include <unistd.h>

int main()
{
	printf("这是父进程，父进程ID：%d\n",getpid());
	sleep(2); 
	printf("创建子进程！\n");
	int n=58;
	int ret=fork();
	if(ret)
	{	
		printf("这是父进程:%d\n,    ret:%d\n",getpid(),ret);
		while(1)
			{
				printf("n:%d\n",n++);
				sleep(1);
			}
	}
	else
	{
		printf("这是子进程:%d\n,     ret:%d\n",getpid(),ret);
		while(1)
			{
				printf("m:%d\n",n+2);
				sleep(1);
			}
	}
}