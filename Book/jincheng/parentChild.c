#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
int main()
{
	//父进程滚动打随机数
	//子进程打印当前时间
	char userName[25]="student";
	char passWord[25]="123456";

	char user[25];
	char pass[25];
	while(1)
	{
		printf("请输入用户名：");
		scanf("%s",user);
		printf("请输入密码:");
		scanf("%s",pass);

		if(0==strcmp(user,userName)&&
			0==strcmp(pass,passWord))
		{
			printf("登陆成功！\n");
			break;
		}
		else
		{
			printf("登陆失败，请重新输入！\n");
			sleep(2);
			system("clear");
		}
	}


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
		}
	}
	else
	{
		//子进程
		//显示随机数
		srand(time(0));
		while(1)
		{
			printf("=============%07d===========\n",
				rand()%10000000);
				usleep(500000);
		}
	}

	return 0;
}