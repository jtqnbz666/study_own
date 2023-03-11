![1645678774408](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\1645678774408.png)poll:
			poll和select非常类似

	1. poll没有最大描述符号的限制
 	2. select在操作描述符号的时候用的是描述符号集合 fd_set
 	3. poll在操作描述符号的时候用的是结构体链表



poll的编程模型：

 1. 创建fd结构体数组
    struct pollfd fds[300];
    int fdNum=0;//当前描述符号数量

 2. 把要监视的描述符号设置好
    fds[0].fd=0;;  
    fds[0].event=POLLIN;
    fdNum++;

 3. 监视
    int r=poll(fds,fdNum,0);
    if(-1==r)错误

    else if(0==r)没有动静
    else {//检查对应事件
    if(fds[0].revents&POLLIN)  {				}

    }

### POLL简单测试案例

~~~c
#include<stdio.h>
#include<poll.h>
#include<string.h>
int main()

{
	struct pollfd fds[10];
	fds[0].fd=0;
	fds[0].events=POLLIN;

	int r;
	char buff[1024];
	while(1)
	{
		r=poll(fds,1,0);
		if(r==-1)printf("监视出错！\n");
		else if(r==0)continue;
		else
		{
			printf("有动静！\n");
			if(fds[0].revents&POLLIN)
			{
				memset(buff,0,1024);
				scanf("%s",buff);
				printf(">>%s\n",buff);
			}
		}
	}
	return 0;

}
~~~

POLL多人聊天服务器端

~~~c
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/select.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#include<poll.h>
#define NUM 100
int serverSocket ,clientSocket[NUM];

void hand()
{
		//  断开连接
	for(int i=0;i<NUM;i++)
	{
		if(-1!=clientSocket[i])
		{
			close(clientSocket[i]);
		}
	}
	close(serverSocket);
	printf("拜拜了老铁！\n");
	exit(0);

}
int main(int argc,char* argv[])
{
	memset(clientSocket,-1,100);
	if(argc!=3)printf("请输入ip地址和端口号！\n"),exit(0);
	printf("ip:%s    port:%d\n",argv[1],atoi(argv[2]));

	signal(2,hand);
	//1.创建socket
    serverSocket=socket(AF_INET,SOCK_STREAM,0);
	if(-1==serverSocket)printf("创建socket失败：%m\n"),exit(-1);
	printf("创建socket成功！\n");

	//2 创建服务器协议地址簇
	struct sockaddr_in addr={0};
	addr.sin_family=AF_INET;  		//协议类型	和socket函数第一个参数一致
	addr.sin_addr.s_addr=inet_addr(argv[1]);//把字符串   转成  整型
	addr.sin_port=htons(atoi(argv[2]));//整型  转成	整型		小端转大端
	

	//3.绑定
	int r=bind(serverSocket,(struct sockaddr*)&addr,sizeof(addr));
	if(-1==r)printf("绑定失败：%m\n"),close(serverSocket),exit(-2);
	printf("绑定成功！\n");

	//4 监听
	r=listen(serverSocket,10);
	if(-1==r)printf("监听失败！:%m\n"),close(serverSocket),exit(-2);
	printf("监听成功！\n");

	//初始化客服端描述

	

	//5 接受客户端的连接
	struct sockaddr_in cAddr={0};
	int len=sizeof(cAddr); 	//等下改成0 测试一下
	int cfd;char buff[1024]; 

	int fdNum=0;
	//监视的fd结构体数组
	struct pollfd fds[NUM];
	for(int i=0;i<NUM;i++)
	{
		fds[i].fd=-1;
		fds[i].events=POLLIN;
	}
	//先把服务器的放进去；
	fds[0].fd=serverSocket;
	fds[0].events=POLLIN;
	fdNum++;
	while(1)
	{
		r=poll(fds,NUM,10);
		if(-1==r)
		{
			printf("监视失败:%m\n"),close(serverSocket),exit(-1);

		}
		else if(0==r){continue;}
		else 
		{
			//检查是否是客户端连接服务器的动作
			if(fds[0].revents&POLLIN)
			{
				//有客户端
				cfd=accept(serverSocket,(struct sockaddr*)&cAddr,&len);
				if(cfd==-1)
				{
					printf("服务器崩溃：%m\n");
					continue;
				}
				printf("有客户端连接到了服务器：%d  - %s\n",cfd,inet_ntoa(cAddr.sin_addr));

				//把客服端的描述符号添加到监视数组中去
				for(int i=0;i<NUM;i++)
				{
					if(-1==fds[fdNum].fd)
					{
						//clientSocket[i]=cfd;
						fds[i].fd=cfd;
						fds[i].events=POLLIN;
						fdNum++;
						break;
					}
				}
			}

			for(int i=1;i<NUM;i++)
			{
				if(-1!=fds[i].fd&&(fds[i].revents&POLLIN))
				{
					r=recv(fds[i].fd,buff,1023,0);
					if(r>0)
					{
						buff[r]=0;
						printf("%d:%s\n",fds[i].fd,buff);
					}
					else
					{
						//客户端掉线
						printf("  客户端 %d 掉线啦！！\n",fds[i].fd);
						fds[i].fd=-1;
						fdNum--;
					}
				}
			}
		}
	}

	return 0;
}   
~~~

epoll：
		select和poll没啥区别		轮询fd，
		效率收到描述符号数量的影响，描述符号越多，效率越低。
		盯着手机，看有没有电话来

​		epoll是优化后的poll，通过注册事件    不再需要轮询，来电话了会有提示。		



  		1. 创建epoll
       epoll_create
          		2. 注册描述符号的事件
       
             epoll_ctl
             		3. 等待，挨个处理事件 
                epoll_wait	等待
                检查  用&   和poll相同

![1645684661567](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\1645684661567.png)

![1645684784241](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\1645684784241.png)

![1645684802930](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\1645696564411.png)

![](C:\Users\ASUS\Pictures\博客图片\QQ图片20220225080602.png)

![](C:\Users\ASUS\Pictures\博客图片\QQ图片20220225080618.png)

### epoll多人聊天室服务器端

~~~c
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/select.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#include<sys/epoll.h>
#define NUM 100
int serverSocket ,clientSocket[NUM];

void hand()
{
		//  断开连接
	for(int i=0;i<NUM;i++)
	{
		if(-1!=clientSocket[i])
		{
			close(clientSocket[i]);
		}
	}
	close(serverSocket);
	printf("拜拜了老铁！\n");
	exit(0);

}
int main(int argc,char* argv[])
{
	memset(clientSocket,-1,100);
	if(argc!=3)printf("请输入ip地址和端口号！\n"),exit(0);
	printf("ip:%s    port:%d\n",argv[1],atoi(argv[2]));

	signal(2,hand);
	//1.创建socket
    serverSocket=socket(AF_INET,SOCK_STREAM,0);
	if(-1==serverSocket)printf("创建socket失败：%m\n"),exit(-1);
	printf("创建socket成功！\n");

	//2 创建服务器协议地址簇
	struct sockaddr_in addr={0};
	addr.sin_family=AF_INET;  		//协议类型	和socket函数第一个参数一致
	addr.sin_addr.s_addr=inet_addr(argv[1]);//把字符串   转成  整型
	addr.sin_port=htons(atoi(argv[2]));//整型  转成	整型		小端转大端
	

	//3.绑定
	int r=bind(serverSocket,(struct sockaddr*)&addr,sizeof(addr));
	if(-1==r)printf("绑定失败：%m\n"),close(serverSocket),exit(-2);
	printf("绑定成功！\n");

	//4 监听
	r=listen(serverSocket,10);
	if(-1==r)printf("监听失败！:%m\n"),close(serverSocket),exit(-2);
	printf("监听成功！\n");

	//5 接受客户端的连接
	struct sockaddr_in cAddr={0};
	int len=sizeof(cAddr);
	int cfd;char buff[1024];
	int  fdNum=0;

	//创建epoll 参数可忽略  给 设置的 最大描述符号数量
	int epfd=epoll_create(NUM);
	if(-1==epfd)
	{
		printf("创建epoll失败：%m\n");
		close(serverSocket);
		exit(-1);
	}
	printf("创建epoll成功！\n");
8+0
	//注册事件
	struct epoll_event ev;//注册时候用
	struct epoll_event events[NUM];//等待 处理事件的时候用

	ev.events=EPOLLIN;
	ev.data.fd=serverSocket;
	r=epoll_ctl(epfd,EPOLL_CTL_ADD,serverSocket,&ev);
	if(-1==r)
	{
		printf("注册serverSocket事件失败！：%m！\n");
		close(epfd);
		close(serverSocket);
		exit(-2);

	}
	printf("注册serverSocket事件成功！\n");

	int currentCfd;//当前发数据过来的客户端描述符号
	int nfds;//epoll_wait的返回值
	while(1)
	{
		nfds=epoll_wait(epfd,events,NUM,1000);
		if(nfds<0)
		{
			printf("epoll_wait失败！：%m\n");
			close(epfd);
			close(serverSocket);
			exit(-3);
		}
		else if(0==nfds)
		{
			continue;
		}
		else
		{
			printf("有动静！========\n");

			for(int i=0;i<nfds;i++)
			{
				if(serverSocket==events[i].data.fd)
				{
					cfd=accept(serverSocket,NULL,NULL);
					if(-1==cfd)
					{
						printf("服务器崩溃：%m\n");
						close(epfd);
						close(serverSocket);
						exit(-4);
					}
					printf("有客户连接上服务器了:%d\n",cfd);
					//注册客户端描述符号
					ev.events=EPOLLIN;
					ev.data.fd=cfd;

					epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&ev);
				}
				else if(events[i].events&EPOLLIN)
				{
					//某个客户端发数据过来了。
					currentCfd=events[i].data.fd;
					r=recv(currentCfd,buff,1023,0);
					if(r>0)
					{
						buff[r]=0;
						printf("%d:%s\n",currentCfd,buff);
					}
				}
			}
		}
	}
	return 0;
}   
~~~

