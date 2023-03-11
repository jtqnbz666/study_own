# UDP协议

### 服务器端代码

~~~C++
#include <iostream>
#include <WinSock2.h>
#pragma comment(lib,"WS2_32.lib")
using namespace std;
int main()
{
	//初始化套接字库资源
	WSADATA data;
	WORD w = MAKEWORD(2, 0);
	WSAStartup(w, &data);
	//创建套接字
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in addr, addr1;
	int n = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1666);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	//绑定
	bind(s, (sockaddr*)&addr, sizeof(addr));
	cout << "UDP  server  启动：" << endl;

	char recvStr[20] = " ";
	char sendStr[20] = " ";
	while (1)
	{
		//发送和接收
		//sendto
		//recvfrom
		if (recvfrom(s, recvStr, 20, 0, (sockaddr*)&addr1, &n) != 0)
		{
			cout << inet_ntoa(addr1.sin_addr) << "发来消息" << endl;
			cout << "client say:" << recvStr << endl;
			cout << "server input:";
			cin >> sendStr;
			sendto(s, sendStr, sizeof(sendStr), 0, (sockaddr*)&addr1, n);
		}
	}
	closesocket(s);
	WSACleanup();
	return 0;
}
~~~

###  客户端代码

~~~c++
#include <iostream>
#include <WinSock2.h>
#pragma comment(lib,"WS2_32.lib")
using namespace std;
int main()
{
	//初始化套接字库资源
	WSADATA data;
	WORD w = MAKEWORD(2, 0);
	WSAStartup(w, &data);
	//创建套接字
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in addr, addr1;
	int n = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1666);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	////绑定，     绑不绑定都没关系
	//bind(s, (sockaddr*)&addr, sizeof(addr));
	cout << "UDP  client启动：" << endl;
	char recvStr[20] = "";
	char sendStr[20] = "";
	while (1)
	{
		cout << "Client input:";
		cin >> sendStr;
		if (sendto(s, sendStr, sizeof(sendStr), 0, (sockaddr*)&addr, n) != 0)
		{
			recvfrom(s,recvStr,20,0,(sockaddr*)&addr,&n);
			cout << "server say:" << recvStr << endl;
		}
	}
	closesocket(s);
	WSACleanup();

	return 0;
}
~~~

