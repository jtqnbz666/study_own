aio:
		文件描述符号可以分为两类：
			网络 io：socketFd
			文件 io：fd		

​		window    所有的描述符号的异步操作   都是   iocp

​		Linux	针对	socketFd	epoll		io多路复用（异步阻塞）
​					针对		fd			aio			异步io（异步非阻塞）



aio 理论：
			涉及到状态切换

​			让io过程异步进行  从而 提高线程读写效率

​			aio执行完毕后  立即返回
​			两种方式来操作需要操作的数据：
​			1.检查
​			2.通知（信号，信号量，回调函数）



aio涉及函数：

​		需要链接库  -lrt



aio案例：
	1.aio_read:异步io

![1645779246783](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\1645779246783.png)

aio编程模型：
		1.准备缓冲区		struct  aiocb cb
		2.异步操作 	异步读	异步写	aio_read 	aio_write
		3.检查是否操作完毕		
				aio_error  拿到数据返回0
				aio_suspend 阻塞式监控
    	4.得到数据		aio_return 	返回读到的数据长度，失败返回-1；

![](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\1645780451880.png)

![1645780421969](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\1645780421969.png)	2.aio_suspend:阻塞等待

![1645782899136](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\1645783842637.png)	3.lio_listio:多个异步io

![1645788428238](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\1645788428238.png)

![1645788439569](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\1645788439569.png)

- 准备缓冲区
  		struct aiocb rcb;
  		struct  aiocb wcb;
  		struct aiocb*  aiocb_list[AIO_LIST_NUM]={0};
- 异步操作
  		异步读	aio_read;
  		异步写	aio_write
- lio_listio检查多个异步操作
- 获取返回值		aio_return 

调用库函数的情况：

windows下

#include<mmsystem.h>
#pragma	comment(lib,"winmm.lib")//库的加载

linux下

aio.c



gcc -c aio.c 
gcc aio.o

gcc -c aio.c
gcc aio.o  -l rt  	//在链接的时候加上库