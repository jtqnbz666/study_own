server.py

```
fromSimpleXMLRPCServer importSimpleXMLRPCServer

deffun_add(a,b):

totle = a + b

returntotle

if__name__ == '__main__':

s = SimpleXMLRPCServer(( '0.0.0.0', 8080)) #开启xmlrpcserver

s.register_function(fun_add) #注册函数fun_add

print"server is online..."

s.serve_forever #开启循环等待
```

Client.py：

```
fromxmlrpclib importServerProxy #导入xmlrpclib的包

s = ServerProxy( "http://172.171.5.205:8080") #定义xmlrpc客户端

prints.fun_add( 2, 3) #调用服务器端的函数
```

![](..\pic\3539721_1585311937406_093a2963d526451f977f8437bdd8b789.jpeg)



rpc是一种技术思想，底层可以用自定义的tcp，udp，也可以用成型的http，比如这里用的http可以通过某一个字段表达，具体的数据以什么编码格式，这里是xml，如果用自定义的应用层协议(不管基于tcp还是udp)，就需要自己设计一个结构体，客户端发送这个结构体，服务器接收这个结构体，从这个结构体中获取内容的编码格式，再进一步进行解析，这里就能看出来，我们需要自己去设计这些，而http帮我们封装好了， 但使用http的缺点就是会有一些没必要的字段，增大网络传输量， 但grpc底层用的是http2.0(二进制压缩)可以很好的减少数据传输量。



- RPC 更侧重于动作。
- REST 的主体是资源。

RESTful 是面向资源的设计架构，但在系统中有很多对象不能抽象成资源，比如登录，修改密码等而 RPC 可以通过动作去操作资源。所以在操作的全面性上 RPC 大于 RESTful

RESTful 是一种设计和构建网络应用程序的软件架构风格。REST（Representational State Transfer）代表一组通过 HTTP 协议进行通信的约束和原则，**以实现统一接口、无状态性、可缓存性、可扩展性和可读性。**

RESTful 架构风格的核心思想是将资源（Resource）作为网络上的统一标识，并通过 HTTP 方法（GET、POST、PUT、DELETE 等）对这些资源进行操作。每个资源都通过唯一的 URL（统一资源定位符）进行标识。而操作资源的状态更改，则通过操作资源的表现形式（例如，通过 JSON、XML 等格式）来传递和传输。

GeeRPC中：核心就是，1. 服务器通过反射来解析客户端的请求，而不是硬编码的方式；2. 实现了注册发现服务来实现负载均衡(主流的有consul，etcd，zookeeper) ；3.自己实现了编码解码方式，而不是直接用的http，其实要用http也是可以的。 4.至于客户端怎么知道调用什么方法完全是无稽之谈，那肯定得客户端知道服务器提供了啥才会去调用，肯定是知道有这个方法并且有目的的去调用的。