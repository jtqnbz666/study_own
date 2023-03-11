## TCP和UDP 在工程上的代码细节区别

他们俩的发送和接收函数是有区别的，在TCP中不需要对方的地址信息，而在UDP中却需要对方的地址信息。

sockaddr_in saddr, caddr; 
saddr 代表服务端协议地址，caddr 代表客户端协议地址

- 在UDP中(客户端)

- sendto(s, sendStr, sizeof(sendStr), 0, (sockaddr*)&saddr, n) ；

- recvfrom(s,recvStr,20,0,(sockaddr*)&caddr,&n)；

- 在UDP中(服务端)

- sendto(s, sendStr, sizeof(sendStr), 0, (sockaddr*)&caddr, n) ；

- recvfrom(s,recvStr,20,0,(sockaddr*)&caddr,&n)；

- 在TCP中

- send(m_usocket, message.c_str(), message.size() + 1, 0);

- recv(m_usocket, message, messageLen, 0);

  

### 重点1：对于客服端部分的代码而言，不管是哪种方式来实现都需要对服务端协议地址进行各参数的设置

  ​	在UDP中sendto()函数需要用到服务端协议地址
  ​	在TCP中connect()函数需要用到服务端协议地址

### 重点2：只有在UDP中发送和接收消息才涉及到对方协议地址参数

- 对于客服端而言，sendto()里面的协议地址参数为服务端协议地址，而recvfrom()里面的协议地址参数为另一个用于接收消息的协议地址。
- 对于服务端而言，sendto()和recvfrom()里面的协议地址参数都只是用来接收发送消息的协议地址，并非服务端的协议地址。

