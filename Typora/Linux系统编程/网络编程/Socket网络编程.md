Linux上网络编程基础：

​			主机：交换机 基站  路由器  电脑  手机 。。

​			连接：无线电   光   电。。

网络服务器一般是unix系统	大小端系统

​		代码中使用端口号，需要 从小端  转换成  大端

TCP协议
		服务器											客户端

1. 创建socket									1.创建socket
2. 创建服务器协议地址簇                  2.获取服务器协议地址簇 
3. 绑定
4. 监听                           
5. 接受连接                                         3.连接服务器
   accept是阻塞函数
6. 通信                                                 4.通信
7. 断开连接                                          5.断开连接



接收端：服务器

1. 等待客户端发送
2. 接受文件名并创建文件
3. 接受文件大小并新建文件
4. 循环接受并写入数据
5. 接受数据足够，关闭文件

UDP 协议
        服务器
1.创建socket										   1.创建socket
2.确定服务器协议地址簇						 2.获取服务器协议地址簇
3.绑定
4.通信													 3.通信

recvfrom =recv + accept;
sendto=send + connect;



TCP和UDP的区别

1. TCP有连接，UDP无连接

2. TCP是数据流，UDP是数据报文

3. TCP收发数据相对慢，UDP收发数据相对快。

4. TCP 安全  稳定   可靠    UDP  不安全  不稳定  不可靠

   ​					安全：数据相对不容易被窃取

   ​					稳定：几乎没有传输速率的变化
   ​					可靠：一定能收到

5. TCP有序，数据有边界，UDP无序，数据无边界

## 单交流TCP  ：

### 服务端：

~~~c
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
int serverSocket ,clientSocket;

void hand()
{
		//  断开连接
	close(clientSocket);
	close(serverSocket);
	printf("拜拜了老铁！\n");
	exit(0);

}
int main(int argc,char* argv[])
{
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
	addr.sin_addr.s_addr=inet_addr(argv[1]);//把字符串   转成  网络字节序整数(二进制数)
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
	struct sockaddr_in cAddr;
	int len=sizeof(cAddr);

	 clientSocket=accept(serverSocket,(struct sockaddr*)&cAddr,&len);
	if(-1==clientSocket)printf("服务器崩溃了！：%m\n"),close(serverSocket),exit(-2);
	printf("有客户端连接上服务器了：%s!\n",inet_ntoa(cAddr.sin_addr));

	//6 通信
	char buff[256];

	while(1)

	{
		r=recv(clientSocket,buff,255,0);
		if(r>0)
		{
			buff[r]=0;//添加结束符
			printf(">>%s\n",buff);
		}
	}
	return 0;
}   
~~~

### 客户端

~~~c
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
int clientSocket;

void hand()
{
		//  断开连接
	close(clientSocket);
	printf("拜拜了老铁！\n");
	exit(0);

}
int main(int argc,char* argv[])
{
	if(argc!=3)printf("请输入ip地址和端口号！\n"),exit(0);
	printf("ip:%s    port:%d\n",argv[1],atoi(argv[2]));

	signal(2,hand);
	//1.创建socket
    clientSocket=socket(AF_INET,SOCK_STREAM,0);
	if(-1==clientSocket)printf("创建socket失败：%m\n"),exit(-1);
	printf("创建socket成功！\n");

	//2 创建服务器协议地址簇
	struct sockaddr_in addr={0};
	addr.sin_family=AF_INET;  		//协议类型	和socket函数第一个参数一致
	addr.sin_addr.s_addr=inet_addr(argv[1]);//把字符串   转成  网络字节序整数(二进制形式)
	addr.sin_port=htons(atoi(argv[2]));//字符型  转成	整型		小端转大端
	

	//3 连接服务器
	int r=connect(clientSocket,(struct sockaddr*)&addr,sizeof(addr));
	if(-1==clientSocket)printf("连接失败！：%m\n"),close(clientSocket),exit(-2);
	printf("连接成功！!\n");
	//6 通信
	char buff[256];
	while(1)
	{
		printf("你要发什么");
		scanf("%s",buff);
		send(clientSocket,buff,strlen(buff),0);
	}
	return 0;
}   

~~~

