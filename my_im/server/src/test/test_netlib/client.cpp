#include "netlib.h"
#include "BaseSocket.h"
#include "EventDispatch.h"
#include <pthread.h>
#include <iostream>

using namespace std;

//因为netlib网络库的回调函数的参数形式就是这样
void* send_msg(void* handle) {
    int fd = *(int*)handle;
    while(1) {
        CBaseSocket* pSocket = FindBaseSocket(fd);
        string str;
        cin >> str;
        str += '\n';
        int ret = netlib_send(fd, (void*)str.c_str(), str.length() + 1);
    }
}
void imconn_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam)
{
   switch(msg) {
    case NETLIB_MSG_READ:
    {
        CBaseSocket* pSocket = FindBaseSocket(handle);
        if(!pSocket) {
        return ;
        }
        char buf[64] = {0};
	    int ret = netlib_recv(handle, buf, 64);
        buf[ret] = '\0';
        //cout << buf ;
    }
    break;
    case NETLIB_MSG_WRITE:
    {
        //
    }
    break;
   }
}

int main() {
    netlib_init();
    net_handle_t handle = netlib_connect("127.0.0.1", 6666, imconn_callback, NULL);
    CBaseSocket* pSocket = FindBaseSocket(handle);
    if(!pSocket) {
        return 0;
    }
    pSocket->SetState(SOCKET_STATE_CONNECTED);
    pthread_t pt;
    pthread_create(&pt, NULL, send_msg, &handle);
    netlib_eventloop(10);
    return 0;
}