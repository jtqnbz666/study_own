## TCP协议

1.TcpSocketS.hpp

~~~c
#pragma once
#include <iostream>
#include <string>
#include <winsock2.h>
#pragma comment (lib,"WS2_32.lib")
using namespace std;
class TcpSocket
{
public:
	TcpSocket():m_usocket(0),m_csocket(0),m_uAddr({0}),m_cAddr({0}),m_clen(0)
	{
		//初始化套接字库
		WSADATA data;
		WORD w = MAKEWORD(2, 0);
		if (WSAStartup(w, &data) != 0)
		{
			cout << "初始化套接字库失败。";
		}
	}
	//服务器操作
	int Bind(u_short port, string ip="")
	{
		//创建套接字
		m_usocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		//初始化ip地址信息
		m_uAddr.sin_family = AF_INET;
		m_uAddr.sin_port = htons(port);
		if (ip.size() == 0)
		{
			m_uAddr.sin_addr.S_un.S_addr = INADDR_ANY;
		}
		else
		{
			m_uAddr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
		}
		//绑定过程
		return bind(m_usocket, (sockaddr*)&m_uAddr, sizeof(m_uAddr));
	}
	int Listen(int num)
	{
		return listen(m_usocket, num);
	}
	//服务器的接受操作
	SOCKET Accept()
	{
		m_clen = sizeof(m_cAddr);
		m_csocket = accept(m_usocket, (sockaddr*)&m_cAddr, &m_clen);
		//错误代码
		//cout<<WSAGetLastError()<<endl;//套接字查问题方法
		return m_csocket;
	}
	//提供一个接口打印ip地址
	string getAddress() const
	{
		return inet_ntoa(m_cAddr.sin_addr);
	}
	//客户端发送消息
	SOCKET SendData(string message)
	{
		return send(m_csocket, message.c_str(), message.size() + 1, 0);
	}
	string RecvData(int messageLen)
	{
		char* message = new char[messageLen];
		memset(message, 0, messageLen);
		recv(m_csocket, message, messageLen, 0);
		string data = string(message);
		delete[] message;
		return data;
	}
	~TcpSocket()
	{
		closesocket(m_usocket);
		closesocket(m_csocket);
		WSACleanup();
	}
protected:
	SOCKET m_usocket;
	sockaddr_in m_uAddr;
	//处理另一台计算机套接字
	SOCKET m_csocket;
	sockaddr_in m_cAddr;
	int m_clen;
};
~~~



2.TcpSocketSever.cpp

~~~c
#include "TcpSocketS.hpp"
int main()
{
	TcpSocket server;//服务器就创建好了。
	server.Bind(1588);
	server.Listen(5);
	cout << "服务器启动:\n";
	if (server.Accept() != INVALID_SOCKET)
		cout << server.getAddress() << "连接上服务器" << endl;
	while (1)
	{
			cout<<"二弟:"<<server.RecvData(1024)<<'\n';
			cout << "大哥：";
			string str;
			cin >> str;
			server.SendData(str);
	}
	return 0;
}
~~~

3.TcpSocketClient.cpp

~~~c
#include "TcpSocketC.hpp"
int main()
{
	TcpSocket client;
	client.Requst(1588);//端口号必须一致
	cout << "客户端启动：\n";
	while (1)
	{
		cout <<"二弟：";
			string str;
			cin >> str;
			client.SendData(str);
			cout <<"大哥:" << client.RecvData(1024)<<'\n';
	}
	return 0;
}
~~~

4.TcpSocketC.hpp

~~~c
#pragma once
#include <iostream>
#include <string>
#include <winsock2.h>
#pragma comment(lib,"WS2_32.lib")
using namespace std;
class TcpSocket
{
public:
	TcpSocket() :m_usocket(0), m_csocket(0), m_uAddr({ 0 }), m_cAddr({ 0 }), m_clen(0)
	{
		//初始化套接字库
		WSADATA data;
		WORD w = MAKEWORD(2, 0);
		if (WSAStartup(w, &data) != 0)
		{
			cout << "初始化套接字库失败！" << endl;
		}
	}


	//客户端的请求操作
	int Requst(int port, string ip = "")
	{
		//创建套接字
		m_csocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		//初始化ip地址信息
		m_uAddr.sin_family = AF_INET;
		m_uAddr.sin_port = htons(port);
		if (ip.size() == 0)
		{
			m_uAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");			   //任何ip地址都可以
		}
		else
		{
			m_uAddr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());  //本地测试
		}
		return connect(m_csocket, (sockaddr*)&m_uAddr, sizeof(m_uAddr));
	}


	//向客户端发送消息
	SOCKET SendData(string message)
	{
		return send(m_csocket, message.c_str(), message.size() + 1, 0);
	}

	string RecvData(int messageLen)
	{
		char* message = new char[messageLen];
		memset(message, 0, messageLen);
		recv(m_csocket, message, messageLen, 0);
		string data = (string)message;
		delete[] message;
		return data;
	}

	~TcpSocket()
	{
		closesocket(m_usocket);
		closesocket(m_csocket);
		WSACleanup();
	}
protected:
	//本身的套接字和本身的ip地址
	SOCKET m_usocket;
	sockaddr_in m_uAddr;

	//处理另一台计算机套接字
	SOCKET m_csocket;
	sockaddr_in m_cAddr;
	int m_clen;
};
~~~

