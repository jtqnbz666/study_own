 cp auto_setup/im_server/conf/msgserver.conf server/src/msg_server/	做gdb调试

先b main -> r -> ctrl + c ->  b CMsgConn::HandlePdu(CImPdu*)->c->在客户端进行登录->bt

HttpParser是第三方库。

BaseSocket中的hash：
typedef hash_map<net_handle_t, CBaseSocket*> SocketMap;

LonginConn中的hash：
typedef hash_map<net_handle_t, CImConn*>		**ConnMap_t**;
static **ConnMap_t** g_client_conn_map;
static **ConnMap_t** g_msg_serv_conn_map;

BaseSocket是socket套接字的封装类，每次accept就new一个，封装了读，写，异常事件

hash不会有冲突问题，因为socket不会一样

EventDispatch 相当于epoll_ctl(增加，移除),epoll_wait,定时事件(增加，移除)，检查定时事件(check timer)等的封装，并且它的实例对象是唯一的，因为用了单例模式，可以说明所有套接字都绑定在一个epoll_wait上

每个listen fd的处理函数m_callback(m_callback_data(回调额外参数), 消息类型，对方给的fd， NULL),
但如果是其他普通fd,这里的第三个参数就是自己的fd,第一个参数m_callback_data一般是一个hash映射表

每一个client连接时，都会给它分配一个BaseSocket对象和一个CLoginConn对象，BashSocket是一些基本信息，比如对方和自己的ip和port，和读写函数(所有的fd都是一样的，在BaseSocket对象的读写函数里面通过fd找到CLoginConn对象再调用CLoginConn对象的具体处理)，但具体的对数据的处理，比如读写缓冲区，解析PDU数据等，都在CLoginConn对象中。

~~~c
当epoll_wait触发了可读事件的整个流程：
pSocket->OnRead();//EventDispatch.cpp中386行
m_callback(m_callback_data, NETLIB_MSG_READ, (net_handle_t)m_socket, NULL); //这个m_callback函数是fd最初设置的回调函数，一般是imconn_callback（除了客户端登录连接）
再进入imconn_callback中可以看到
CImConn* pConn = FindImConn(conn_map, handle);//通过fd取出Conn对象，CImConn是所有Conn对象的基类， 这里我们是CLoginConn对象，
pConn->OnRead();//里面会调用HandlePdu，这是一个子类重写的方法，所以会去到CLoginConn对象的HandlePdu处理方法，这里面就是具体对PDU的处理

~~~



对于imconn.cpp, 每个客户来连接Login_server时，绑定的回调函数就是这里面的imconn_callback, 同时给客户设置的callback_data就是在LoginConn.cpp中的g_client_conn_map哈希表，所以在客户的回调函数中，可以通过g_client_conn_map找到对应的那条连接。我们可以把CLoginConn的一个对象当作一个真正的连接

在BaseSocket.cpp中， g_socket_map哈希表保存了每一个fd的映射(fd->CBaseSocket*)，
在LoginConn.cpp中, 有g_client_conn_map和 g_msg_serv_conn_map 两个不同的哈希表,分别用来映射普通客户的连接, 和msg_server服务器那边来的连接对象,目前还没发现g_client_conn_map 在哪里进行了客户的插入.



netlib_option(客户fd，需要进行的操作(如绑定回调函数，设置回调函数附加的参数等)，具体的回调函数和回调函数附加的参数(一般是一个hash表), 并且在netlib.cpp中包含了BaseSocket头文件,等同于拥有了g_socket_map哈希表, 所以通过这个哈希表获得对应的socket才能对回调函数即额外参数进行设置



对于login_server.cpp， 只有一个epoll_wait， 但涉及到**多个服务**，比如普通的client连接，以及http消息，这些服务对应的listenfd绑定了EPOLLIN事件，当事件发生时，先通过FindBaseSocket(ev_fd), 找到对应的listenfd的pSocket对象，然后调用pSocket->OnRead();

对于OnRead()里面有哪些消息类型，存储在ostype.h里面
对于Socket的回调函数中第二个参数涉及的类型在netlib.h中

目前看来，int netlib_option(net_handle_t handle, int opt, void* optval) 中， 如果opt是NETLIB_OPT_SET_CALLBACK，则optval是一个函数， 如果opt是NETLIB_OPT_SET_CALLBACK_DATA，则optval是一个全局hash表.

在CMsgConn::HandlePdu断点查看, 搞不清楚就去看懂最简单的客户端和服务器的连接



### 第二阶段

在CMsgConn::HandlePdu断点查看, 搞不清楚就去看懂最简单的客户端和服务器的连接。

对于一开始的登录， 去MsgConn.cpp 中看_HandleLoginRequest。注意：所有的这一类处理操作， **都需要先经过CMsgConn::HandlePdu判断CommandId类型**， 再决定去调用哪个具体的处理函数

xxxServerConn代表自己(一般是msg_server)去连接其他服务

xxxConn 代表其他客户端、服务端，来连接我们自己。

如果是别人来连我，对应的逻辑就去xxxConn；如果是我连别人，就去xxxServerConn看， 注意都是从HandlePdu看起

在IM.Server.proto第28行可以看到 attach_data。

在httpconn.cpp（login_server服务器的）中可以看到如何做负载均衡分给msg_server的.

Connfirm函数如果触发了，表示需要连接的服务连接成功

对于db_proxy_server 是在xxxAction.cpp中处理pDu并返回结果的，而且db_proxy_server把服务分为了单独的模块进行处理， 比如部门列表，会话消息等等，分为了xxxModel.cpp类型的模块，一般其他服务都是在对应Conn连接中的HandlePdu进行处理的，这里需要注意一下。

attach_data实质上是绑定了一个m_handle。

在epoll_v7.c 是直接一个socket就绑定好了一个处理函数(C语言风格);

在IM中， 通过统一的BaseSocket对象进入总体的回调函数， 找到对应的连接(Conn),根据参数，再调用具体的回调方法,BaseSocket完成一些基础功能，比如对socket的设置等，具体的解析数据等操作在对应的Conn中进行(HandlePdu)。

**重点：整理思路：**

先在服务器listen(会调用netlib_listen(), 分配BaseSocket，并且绑定listenfd对应的m_callback回调函数(假设我们这里指的是普通对象的处理，则对应client_callback回调函数))几个需要的服务(比如http服务需要的监听套接字，普通连接服务需要的监听套接字， msg_server连接过来的监听套接字)，并使用AddEvent将这几个listenfd放入epoll的事件循环中， 当有读消息到来时(比如第一个普通客户连接过来)，通过FindBaseSocket(普通连接服务对应的监听套接字)，找到BaseSocket对象， 调用BaseSocket->OnRead。
这个函数里有两种情况，

~~~c
void CBaseSocket::OnRead() 	//伪代码
	if (m_state == SOCKET_STATE_LISTENING)
		_AcceptNewSocket();		
	else
		m_callback(m_callback_data, NETLIB_MSG_READ, (net_handle_t)m_socket, NULL);
~~~



 **1**：若是listenfd进去， **会进一步**进入_AcceptNewSocket完成对新连接的一系列准备工作(比如绑定，分配BaseSocket对象)，后(在_AcceptNewSocket中)**再进一步**调用listenfd对应的m_callback(当前这种情况指client_callback)给这个新连接分配Conn对象,调用Conn->OnConnect2(完成进一步初始化工作(比如给新对象设置真正的回调函数，一般是将m_callback设置为imconn_callback, 以及一个callback_data,一般是一张全局的hash映射表，后边可以通过这个映射表(传fd)来找到对应的conn连接)，这里需要注意我们使用了m_callback, 但m_callback是BaseSocket对象中的，所以在这个函数中我们进一步调用了netlib_option

~~~c
int netlib_option(net_handle_t handle, int opt, void* optval)
~~~

第一个参数是fd， 第二个参数是要进行的操作，比如设置m_callback,或者设置m_callback_data； 如果是设置m_callback, 则第三个参数optval就是一个函数指针， 如果是设置m_callback_data,则第三个参数就是一个哈希表。

在这个函数中我们需要通过

~~~c
CBaseSocket* pSocket = FindBaseSocket(handle);
~~~

找到对应的pSocket对象，才能进行进一步的m_callback以及m_callback_data的设置。
 **2**：若是普通fd进去， 因为这个普通fd一定是经过listenfd处理好了的(意思也就是这个普通fd的callback，以及callback_data都是设置好的)，会直接调用这个callback(callback具体为imconn_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam),)， 第一个参数一般是全局哈希表， 第二个是要进行的操作(比如读取，或者写)， 第三个就是自己的fd， 第四个是一些额外的参数， 在这里面会通过第一个参数(哈希表)找到对应的Conn连接， 通过第二个参数的不同，来调用不同的具体执行函数(Conn->OnRead .. Conn->OnWrite) 等等，这里**需要注意**，我写的Conn连接有很多种不同的Conn，比如LoginConn， MsgConn等等，但他们的共同点都是继承于CImConn对象。

 细节点就来了：

~~~c
	ConnMap_t* conn_map = (ConnMap_t*)callback_data;
	CImConn* pConn = FindImConn(conn_map, handle);
~~~

我们是通过基类指向子类对象， 在进入pConn->OnRead中后，会调用netlib_recv接口读取数据，进一步封装成为CImPdu对象， 紧接着会在pConn->OnRead中**调用HandlePdu**, 这里的HandlePdu，对于基类，只提供了虚函数， 不同的Conn(比如LoginConn， MsgConn等)对象有不同的HandlePdu处理，比如当前的情况，就会在LoginConn连接中的HandlePdu对数据进行分析处理。

注意：不要把BaseSocket中的m_callback 和 Conn中的各种回调搞混了，上边提到的m_callback都是指的BaseSocket中的。 Conn->OnRead / Conn->OnWrite等才是Conn对象具有的具体功能函数。可以把BaseSocket中的m_callback理解为一层壳。通过BaseSocket中的m_callback的传参不同，调用不同的Conn->系列函数。

调试db_proxy_server 服务：

~~~c
通过gdb attach + 进程号 即可进入调试
b CProxyConn::SendResponsePduList()
c
bt
可以发现在proxy_loop_callback中调用了这个函数

引申， 为什么一般线程池只处理逻辑， 实际的收发任务都在主线程进行， 因为如果一个连接突然断开， 只有主线程才知道， 而线程池中的线程并不知道，所以让一个统一的线程处理收发， 线程池的线程只处理逻辑

c
disable 1	//清除断点的意思
c
ctrl+c

// 继续调试找CMessageModel::getMsgId(unsigned int)
b  CMessageModel::getMsgId(unsigned int)
c
//此处打开程序发个消息
bt 
可以发现在DB_PROXY::sendMessage中调用

disable 2 //清楚断点2
c
ctrl+c

//继续调试
b CUserModel::clearUserCounter(unsigned int, unsigned int, IM::BaseDefine::SessionType)
    
c
//此处打开一个未读消息，就会进去
bt

disable 3 //清除断点3
c
ctrl+c
b CGroupMessageModel::incMessageCount(unsigned int, unsigned int)

c
//此处发送一条群消息
//机会进来， 此处可以输入命令 n  来一行一行看代码
//比如输6个n后
使用 print strGroupKey 查看变量内容
~~~

//优化， 会话信息(如日期) 没有消息重要可以放到redis里面提高速度

​	//把定时器的设计改为set

****



- 对于登录连接。因为用的是http连接，所以采用的是json格式充当消息体返回给客户端，没有用protobuf
- fd有一个回调函数， 任务也有一个回调 



对于msg_serv，每次启动都会主动去连接其他服务器，去连接的每一个其他服务器都对应了一个sockfd，这个sockfd刚开始状态为SOCKET_STATE_CONNECTING，第一次触发的write事件中会调用一次完成认证，然后将状态设置为SOCKET_STATE_CONNECTED，对于msg_serv连接file_serv（Confirm()去获取对方服务器的ip 和 port信息）。

