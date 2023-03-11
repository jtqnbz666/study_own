**slog 分析：**

本质是对log4cpp的封装。

CSLog
teamtalk日志类，利用内部成员(CSLogObject* m_log)调用CSLogObject接口(组合)

CSLogObject
日志对象虚基类，内部接口全为虚函数(实现为空)，主要由子类去实现

CLog4CXX
继承于CSLogObject，利用内部成员(LoggerPtr m_logger)的log4cxx接口重写父类的接口，在util定义了CSLog g_imlog全局变量，实现全局唯一(一般日志类都是单例)。

实质上都是使用了log4cxx中的 LoggerPtr对象进行封装。



### UtilPdu分析

**CSimpleBuffer类**

 uchar_t* GetBuffer()： 获取该string的字符串形式，类似std::string c_str()
uint32_t GetWriteOffset()： 获取已经使用的大小
void Extend(uint32_t len): 拓展len空间(realloc)，实际会再额外拓展1/4大小，如传参32会拓展到40字节
uint32_t Write(void* buf, uint32_t len): 从buf写入len字节数据
uint32_t Read(void* buf, uint32_t len): 读出len字节数据到buf

**CByteStream类**

实现了字节数组和uint16_t int6_t uint32_t int32_t等数字的相互转换。
 比如：一个uint16_t数字转换成2个字节表示的uint8_t数组

CByteStream内部提供了许多static方法，方便外部直接使用

**这个类主要是拿来处理头部信息的。**

**CPduException类**

用于处理异常



### ostype.h

存放了， enum
{
	NETLIB_MSG_CONNECT = 1,
	NETLIB_MSG_CONFIRM,
	NETLIB_MSG_READ,
	NETLIB_MSG_WRITE,
	NETLIB_MSG_CLOSE,
	NETLIB_MSG_TIMER,
	NETLIB_MSG_LOOP,
};

**ProxyConn.cpp**

对于Proxy_server采用线程池来处理任务，对于这里面的各种处理函数都放在HandlerMap中(如DB_PROXY::doLogin)在。

日志分析 

~~~c
进入，auto_setup/im_server-1.0
  输入  tail -n 30 -f msg/server/log/default.log
~~~

