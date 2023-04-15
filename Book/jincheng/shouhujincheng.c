#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>    
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
int main()
{
	//1.重设文件权限           umask
	umask(0);
	//2.创建子进程			   fork
	int ret=fork();
	if(ret<0)printf("创建进程失败：%m"),exit(-1);
	if(ret>0)printf("父进程结束：%m\n"),exit(0);//3.让父进程结束
	if(0==ret)
	{
		printf("pid:%d\n",getpid());
	//4.创建新会话				setsid
	setsid();
	//5.防止子进程成为僵尸进程   	忽略掉SIGCHLD   SIGHUP信号
	signal(SIGCHLD,SIG_IGN);
	signal(SIGHUP,SIG_IGN);
	//6.改变当前工作目录  	chdir
	chdir("/");
	//7.重定向文件描述符号    open  dup2(fd,0)   dup2(fd,1)
	int fd=open("/dev/NULL",O_RDWR);
	dup2(fd,0);
	dup2(fd,1);
	}
	while(1)
	{
		sleep(1);//模拟守护进程工作
	}
	return 0;
}