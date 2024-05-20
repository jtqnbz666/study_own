最开始先写base目录中的ConfigFileReader, ostype, lock, task, util, utilPdu,

先拆线程池，连接池(

base目录中：thread, threadpool， db_proxy_server中：CachePool,DBPool, hiredis, read.h, sds.h,准备三个库libbase.a, libhiredis.a, libslog.so， .so文件记得放入/usr/local/lib 或者 /usr/lib, 最好.a,.so都放，不然需要在cmakelist.txt自己配置路径

 )

写网络库netlib，依赖BaseSocket, EventDispatch

准备写ProxyConn部分

先写Imconn部分， 

~~~
依赖ImPduBase， ImPudBase的实现是依赖utilpdu的，
并且需要IM.BaseDefine.pb.h,
学到了，有两个buf， 一个用于读，一个用于写，


~~~

观察proxyConn部分

~~~
1.依赖Proxytask, 学到了 fd有一个回调函数, 任务也有回调函数，定时器的回调函数参数类型和fd的回调函数一样
2.依赖Handlermap
3.因为依赖SyncCneter，所以先去写它
~~~

我尝试着编译im的libbase.a 但总是遇到protobuf相关的问题，经过排除，是由于我自己apt install protoc 生成了高版本的protoc 而我的项目中的protobuf提供的include，lib资源是低版本的，并且，由于/bin中已经有了高版本的protoc， 我的脚本直接把低版本的protoc放到/bin会失败，但并没有提示，就是这里卡了很久， 需要手动删除高版本的protoc(即先删除/bin/protoc，再将低版本的移过去) 

发现的第二个细节，在解决上述问题后，我尝试在本机上用build_ubuntu.sh脚本，但还是失败，原因是可能在某个文件中的cmake出了问题，根据提示找到它，把cmake生成的中间文件全删了重新执行脚本即可解决

写SyncCenter部分

~~~c
但它依赖了一些base目录
去写这些依赖的文件
如public_define.h，Condition.h, httpclient,
这里开始要用到json, curl 库 了
~~~

接着可以开始写ProxyConn部分了

写完后写db_proxy_server.cpp 

~~~c
又要依赖base目录的EncDec，base64，
发现db_proxy_server/business 下的login服务又依赖base目录下的TokenValidator（token验证器，做一些认证工作）
~~~





## 第二阶段写login_server

从依赖文件最少的LoginConn入手

再写ipparse

再写HttpConn， 但依赖base目录中的httpparser 和 httpparserwrapper， 但不依赖hhtpclient，注意区分它们的关系，这里直接拷贝httpparser和httpparserwrapper。

可以将HttpConn和ImConn对比，它们很像

~~~c
HttpConn中是用conn_handle(一个递增的数字，不是fd)作为hash表的key来对应httpconn连接的，可以防止因fd重用引起的一些冲突

这里引申出了一个智能指针的用法，
 netlib_option(handle, NETLIB_OPT_SET_CALLBACK_DATA, reinterpret_cast<void*>(m_conn_handle));
我个人觉得可以把reinterpret_cast<void*>(m_conn_handle));换成(void*)(&m_conn_handle);
~~~

写完写ipparser,最后写login_server

## 第三阶段写route_server

先写UserInfo

写routeconn

发现route_Server就是和msgser交互

写rout_eserve.cpp

| 时间段      | 任务                 | 是否完成 |
| ----------- | -------------------- | -------- |
| 8:00-11:00  | 看教资               | 否       |
| 11:00-12:00 | 看王鹤棣             | 是       |
| 12:00-2:30  | 做饭干饭洗碗         | 是       |
| 2:30-3:00   | 看王鹤棣             | 是       |
| 3:00-5:00   | 跳舞                 | 否       |
| 5:00-5:30   | 看王鹤棣             | 是       |
| 5:30-7:30   | 做饭干饭洗碗         | 是       |
| 7:30-8:30   | 看王鹤棣             | 是       |
| 8:30-10:30  | 看教资，看完看王鹤棣 | 否       |





## 第四阶段写file_server

先写config_util，但依赖base目录中的singleton.h

写file_server_util.h

写offline_file_util.h

写transfer_task

写transfer_task_manager，虽然依赖file_client_conn，不管(file_client_conn也依赖transfer_task_manager）。

写file_client_conn，file_client_conn.cpp又依赖base目录中的im_conn_util.h

写file_msg_server_conn

写file_server.cpp

## 第五阶段写msg_server

写AttachData,

写ImUser

把所有conn相关文件写了

写FileHandle

写msg_server.cpp

写CMakeLists.txt时发现需要base目录中的security目录

依赖base目录中的ServInfo.hs

写GroupChat 和 jsonxx

写msg_server.cpp

## 第六阶段写http_msg_server

AttachData和msg_server中的一样，

HttpConn和Login_server中的大同小异，需要做一些修改

HttpConn需要用到HttpQuery，去写，发现依赖HttpPdu

可以从HttpPdu中学习jsoncpp的使用方法‘，还依赖RouteServConn和DBServConn,所以先写这两个Conn，

写完就可以继续写HttpConn了， 

写http_msg_server.cpp

## 第七阶段写msfs

写Portable.h

写CriticalSection.h

写StringUtils

写FileLin

写FileManager

写HttpConn(主要是其中的OnUpload，OnDownload)

写main.cpp

## 第八阶段写shell脚本完成自动化建立项目