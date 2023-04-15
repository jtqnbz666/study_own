//parentChild.c文件名
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include <fcntl.h>
int main()
{
	//父进程滚动打随机数
	//子进程打印当前时间
	char userName[25]="student";
	char passWord[25]="123456";

	char user[25];
	char pass[25];
    int fd=open("stu.dat",O_CREAT|O_RDWR);
	if(-1==fd)printf("打开文件失败：%m\n"),exit(-1);
	printf("打开文件成功！\n");

	//2.修改文件大小
	ftruncate(fd,3);
	//3.映射虚拟内存
	int *pp=mmap(NULL,3,
		PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
	if((void*)-1==pp)
		printf("映射虚拟内存失败：%m"),close(fd),exit(-1);
	printf("映射虚拟内存成功!\n");


	//4.使用内存
	int *ppp=pp;
   	*ppp=1;
	printf("写入数据完毕！\n");
	


	if(fork())
	{
		//父进程
		//打印当前时间
		struct tm* pt;
		time_t tt;

		while(1)
		{
			tt=time(0);
		pt=localtime(&tt);
		printf("时间：%02d:%02d:%02d\n",
			pt->tm_hour,pt->tm_min,pt->tm_sec);
		sleep(1);
		
            if(getc(stdin))
            	while(1)
            	{
            		
            		*ppp=0;
            		if(getc(stdin))
            			{*ppp=1;
            			break;}
            	}
		}
	}
	else
	{
		//子进程
		//显示随机数
		srand(time(0));
		while(1)
		
		{
			while(*ppp)
		{
			printf("=============%07d===========\n",
				rand()%10000000);
				usleep(500000);
		}
		}
	}

	return 0;
}