io多路复用：select	poll	epol
		io： 
				input
				output
				输入输出设备		终端

​				serverSocket		
​				fd  0 1 2 

阻塞
		聊天室
		两个阻塞：
			accept  serverSocket  返回  clientSocket
			recv  	对应的clientSocket

轮询式：select  poll
		循环不断去读	serverSocket   clientSocket
				读到了	返回数据
				没读到	返回  -1

​		实时性好点	同时能处理的描述符号数量少     300

中断式(事件式)：epoll	_kbhit			30000
		如果有动静，触发事件

![1645598010518](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\1645598010518.png)

### 多人聊天室服务器端

```
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

	//初始化

	fd_set fds;//描述符号集合
	int maxFd;

	//5 接受客户端的连接
	struct sockaddr_in cAddr={0};
	int len=sizeof(cAddr);
	int cfd;char buff[1024];
		maxFd=0;
		maxFd=((maxFd>serverSocket)?maxFd:serverSocket);
	while(1)
	{
	
		FD_ZERO(&fds);//清空

		FD_SET(serverSocket,&fds);
		//把客户端的socketFd放到监视集合中
		for(int i=0;i<NUM;i++)
		{
			if(-1!=clientSocket[i])
			{
				FD_SET(clientSocket[i],&fds);
			}
		}
		//监视
		r=select(maxFd+1,&fds,NULL,NULL,NULL);
		if(-1==r)
		{
			printf("服务器崩溃：%m!\n");
			close(serverSocket);
			exit(-1);
		}
		else if(0==r)
		{
			printf("正在拼命加载中，请等待...\n");
			continue;
		}
		else 
		{
			//检查是不是serverSocket的动静
			if(FD_ISSET(serverSocket,&fds))
			{
				cfd=accept(serverSocket,NULL,NULL);
				if(-1==cfd)
				{
					printf("客户端连接失败！\n");
				}
				else 
				{
					printf("客户端连接上服务器了！:%d\n",cfd);

					//保存客户端描述符号
					for(int i=0;i<NUM;i++)
					{
						if(-1==clientSocket[i])
						{
							clientSocket[i]=cfd;

							//最大描述符号变化
							maxFd=((maxFd>cfd)?maxFd:cfd);
							break;
						}
					}

				}
			}
									//检查客户端是否有动静

					for(int i=0;i<NUM;i++)
				{
					if(-1!=clientSocket[i]&&FD_ISSET(clientSocket[i],&fds))
					{
						printf("你好！");
						r=recv(clientSocket[i],buff,1023,0);
						if(r>0)
						{
							buff[r]=0;
							printf("%d:%s\n",clientSocket[i],buff);
						}
						else 
						{
							printf("客户端：%d已经断开连接了！\n",clientSocket[i]);
							clientSocket[i]=-1;
						}
					}
				}
		}
				

	}

	return 0;
}   
```

