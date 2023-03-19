#include "netlib.h"
#include "BaseSocket.h"
#include "EventDispatch.h"
#include <iostream>
#include <pthread.h>

using namespace std;
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
void imconn_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam) {
    switch(msg) {
    case NETLIB_MSG_READ:
    {
        CBaseSocket* pSocket = FindBaseSocket(handle);
        char buf[64] = {0};
        int ret = netlib_recv(handle, buf, 64);
        buf[ret] = '\0';
        cout << buf;
        //ret = netlib_send(handle, buf, ret);
        //cout << ret << endl;
    }
    break;
    case NETLIB_MSG_WRITE:
    {
        //
    }
    break;
   }
}
void serv_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam)
{
    if(msg == NETLIB_MSG_CONNECT) {
        netlib_option(handle, NETLIB_OPT_SET_CALLBACK, (void*)imconn_callback);
        CBaseSocket* pSocket = FindBaseSocket(handle);
        if(!pSocket) {
            return ;
        }
        pSocket->SetState(SOCKET_STATE_CONNECTED);
        pthread_t pt;
        pthread_create(&pt, NULL, send_msg, &handle);
    }
}

int main() {
    netlib_init();
    int handle = netlib_listen("127.0.0.1", 6666, serv_callback, NULL);
    netlib_eventloop(10);
    return 0;
}