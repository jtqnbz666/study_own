#include "BaseSocket.h"
#include "EventDispatch.h"
/*
send, sendmsg, recv, recvmsg, accept, connect 。
这两个选项设置后，若超时， 返回-1，并设置errno为EAGAIN或EWOULDBLOCK conncet（）, errno == EINPROGRESS表示正在建立链接
*/
//148行细节点， 写事件触发不一定是连接成功
//系统调用的errno 和 getsockopt 中的errno不是一个意思
typedef hash_map<net_handle_t, CBaseSocket*> SocketMap;
SocketMap  g_socket_map;

void AddBaseSocket(CBaseSocket* pSocket) {
    g_socket_map.insert(make_pair((net_handle_t)pSocket->GetSocket(), pSocket));
}

void RemoveBaseSocket(CBaseSocket* pSocket) {
    g_socket_map.erase((net_handle_t)pSocket->GetSocket());
}

CBaseSocket* FindBaseSocket(net_handle_t fd) {
    CBaseSocket* pSocket = NULL;
    SocketMap::iterator iter = g_socket_map.find(fd);
    if(iter != g_socket_map.end()) {
        pSocket = iter->second;
        pSocket->AddRef(); //增加引用计数，保护BaseSocket对象,并不是fd，当减到0时才销毁
    }
    return pSocket;
}

CBaseSocket::CBaseSocket() {
    m_socket = INVALID_SOCKET;
    m_state = SOCKET_STATE_IDLE;
}
CBaseSocket::~CBaseSocket() {
    //log("CBaseSocket::~CBaseSocket, socket = %d\n", m_socket);
}

int CBaseSocket::Listen(const char* server_ip, uint16_t port, callback_t callback, void* callback_data) {
   m_local_ip = server_ip;
   m_local_port = port;
   m_callback = callback;
   m_callback_data = callback_data;

   m_socket = socket(AF_INET, SOCK_STREAM, 0);
   if(m_socket == INVALID_SOCKET) {
        log_error("socket failed, err_code=%d, server_ip = %s, port = %u", _GetErrorCode(), server_ip, port);
        return NETLIB_ERROR;
   } 
   _SetReuseAddr(m_socket);
   _SetNonblock(m_socket);
   
   sockaddr_in serv_addr;
   _SetAddr(server_ip, port, &serv_addr);
   int ret = ::bind(m_socket, (sockaddr*)&serv_addr, sizeof(serv_addr));
   if(ret == SOCKET_ERROR) {
        log_error("bind failed, err_code=%d, server_ip=%s, port=%u", _GetErrorCode(), server_ip, port);
		closesocket(m_socket); // 就是  close(m_socket);
		return NETLIB_ERROR;
   }

   ret = listen(m_socket, 64);
   if(ret == SOCKET_ERROR) {
        log_error("listen failed, err_code=%d, server_ip=%s, port=%u", _GetErrorCode(), server_ip, port);
		closesocket(m_socket);
		return NETLIB_ERROR;
   }
   
   m_state = SOCKET_STATE_LISTENING;    //记录状态，根据状态调用不同的函数，如果是Listening状态则调用_AcceptSoce
   log_debug("CBaseSocket::Listen on %s:%d", server_ip, port);
   AddBaseSocket(this);
   CEventDispatch::Instance()->AddEvent(m_socket, SOCKET_READ | SOCKET_EXCEP);
   return NETLIB_OK;
}

net_handle_t CBaseSocket::Connect(const char* server_ip, uint16_t port, callback_t callback, void* callback_data) {
    log_debug("CBaseSocket::Connect, server_ip = %s, port = %d", server_ip, port);
    m_remote_ip = server_ip;
    m_remote_port = port;
    m_callback = callback;
    m_callback_data = callback_data;
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(m_socket == INVALID_SOCKET) {
        log_error("socket failed, err_code=%d, server_ip=%s, port=%u", _GetErrorCode(), server_ip, port);
		return NETLIB_INVALID_HANDLE;
    }
    _SetNonblock(m_socket);
    _SetNoDelay(m_socket);
    sockaddr_in serv_addr;
    _SetAddr(server_ip, port, &serv_addr);

    //因为设置成非阻塞，所以可能连接不成功
    int ret = connect(m_socket, (sockaddr*)&serv_addr, sizeof(serv_addr));
    if( (ret == SOCKET_ERROR) && (!_IsBlock(_GetErrorCode())) ) {
        log_error("connect failed, err_code=%d, server_ip=%s, port=%u", _GetErrorCode(), server_ip, port);
		closesocket(m_socket);
		return NETLIB_INVALID_HANDLE;
    }
     //这里connecting 不是 connected;
    m_state = SOCKET_STATE_CONNECTING; //这个意思就表示正在连接，但不一定会成功，在149行更改未connected;
    AddBaseSocket(this);
    CEventDispatch::Instance()->AddEvent(m_socket, SOCKET_ALL);

    return (net_handle_t)m_socket;
}

int CBaseSocket::Send(void* buf, int len) {
    if(m_state != SOCKET_STATE_CONNECTED) { //第一次写事件触发时会执行confirm函数转换为这个状态
        return NETLIB_ERROR;
    }
    int ret = send(m_socket, (char*)buf, len, 0);
    if(ret == SOCKET_ERROR) {
        int err_code = _GetErrorCode();
        if(_IsBlock(err_code)) { //拥塞的情况
            ret = 0;
        } else {
            log_error("send failed, err_code = %d, len = %d", err_code ,len);
        }
        
    }
    return ret;
}

int CBaseSocket::Recv(void* buf, int len) {
    return recv(m_socket, (char*)buf, len, 0);
}

int CBaseSocket::Close() {
    CEventDispatch::Instance()->RemoveEvent(m_socket, SOCKET_ALL);
    RemoveBaseSocket(this);
    closesocket(m_socket);
    //我是这么理解的，尽管关闭了m_socket, 但BaseSocket对象依然存在，资源信息还在
    ReleaseRef();
    return 0;
}

void CBaseSocket::OnRead() {
    if(m_state == SOCKET_STATE_LISTENING) {
        _AcceptNewSocket();
    } else {
        u_long avail = 0;
        //FIONREAD设置最多能读到多少数据，返回值返回设置后最多能读到的数据量
        int ret = ioctlsocket(m_socket, FIONREAD, &avail);
        if( (SOCKET_ERROR == ret) || (avail == 0) ) {
            m_callback(m_callback_data, NETLIB_MSG_CLOSE, (net_handle_t)m_socket, NULL);
        } else {
            m_callback(m_callback_data, NETLIB_MSG_READ, (net_handle_t)m_socket, NULL);
        }
    }
}

void CBaseSocket::OnWrite() {
    if(m_state == SOCKET_STATE_CONNECTING) {
        int error = 0;
        socklen_t len = sizeof(error);
        //判断是否connect成功了， 因为fd是非阻塞的。
        getsockopt(m_socket, SOL_SOCKET, SO_ERROR, (void*)&error, &len);
        
        if(error) {
            m_callback(m_callback_data, NETLIB_MSG_CLOSE, (net_handle_t)m_socket, NULL);
        } else {
            //在这里更改状态为 connected
            m_state = SOCKET_STATE_CONNECTED;
            //注意这里是 NETLIB_MSG_CONFIRM
            m_callback(m_callback_data, NETLIB_MSG_CONFIRM, (net_handle_t)m_socket, NULL);
        }
    } else {
        m_callback(m_callback_data, NETLIB_MSG_WRITE, (net_handle_t)m_socket, NULL);
    }
}

void CBaseSocket::OnClose() {
    m_state = SOCKET_STATE_CLOSING;
    m_callback(m_callback_data, NETLIB_MSG_CLOSE, (net_handle_t)m_socket, NULL);
}

void CBaseSocket::SetSendBufSize(uint32_t send_size) {
    int ret = setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, &send_size, 4);
    if(ret == SOCKET_ERROR) {
        log_error("Set SO_SNDBUF failed for fd = %d", m_socket);
    }
    socklen_t len = 4;
    int size = 0;
    getsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, &size, &len);
    log_debug("socket=%d send_buf_size=%d", m_socket, size);
}

void CBaseSocket::SetRecvBufSize(uint32_t recv_size) {
    int ret = setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, &recv_size, 4);
    if(ret == SOCKET_ERROR) {
        log_error("set SO_RCVBUF failed for fd = %d", m_socket);
    }

    socklen_t len = 4;
    int size = 0;
    getsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, &size, &len);
    log_debug("socket = %d recv_buf_size = %d", m_socket, size);
}

int CBaseSocket::_GetErrorCode() {
    return errno;
}

bool CBaseSocket::_IsBlock(int error_code) {
    return ( (error_code == EINPROGRESS) || (error_code == EWOULDBLOCK));
}

void CBaseSocket::_SetNonblock(SOCKET fd) {
    int ret = fcntl(fd, F_SETFL, O_NONBLOCK | fcntl(fd, F_GETFL));
    if(ret  == SOCKET_ERROR) {
        log_error("_SetNonblock failed, err_code = %d, fd = %d", _GetErrorCode(), fd);
    }
}

void CBaseSocket::_SetReuseAddr(SOCKET fd) {
    int reuse = 1;
    int ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse));
    if(ret == SOCKET_ERROR) {
        log_error("_SetReuseAddr failed, err_code=%d, fd=%d", _GetErrorCode(), fd);
    }
}

void CBaseSocket::_SetNoDelay(SOCKET fd) {
    int nodelay = 1;
    int ret = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char*)&nodelay, sizeof(nodelay));
    if(ret == SOCKET_ERROR) {
        log_error("_SetNoDelay failed, err_code=%d, fd=%d", _GetErrorCode(), fd);
    }
}

void CBaseSocket::_SetAddr(const char* ip, const uint16_t port, sockaddr_in* pAddr) {
    memset(pAddr, 0, sizeof(sockaddr_in));
    pAddr->sin_family = AF_INET;
    pAddr->sin_port = htons(port);
    pAddr->sin_addr.s_addr = inet_addr(ip);
    if(pAddr->sin_addr.s_addr == INADDR_NONE) {
        hostent* host = gethostbyname(ip);
        if(host == NULL) {
            log_error("gethostbyname failed, ip = %s, port = %u", ip, port);
            return;
        }
        pAddr->sin_addr.s_addr = *(uint32_t*)host->h_addr;
    }
}

void CBaseSocket::_AcceptNewSocket() {
    SOCKET fd = 0;
    sockaddr_in peer_addr;
    socklen_t addr_len = sizeof(sockaddr_in);
    char ip_str[64];
    while( (fd = accept(m_socket, (sockaddr*)&peer_addr, &addr_len)) != INVALID_SOCKET) {
        CBaseSocket* pSocket = new CBaseSocket();
        uint32_t ip = ntohl(peer_addr.sin_addr.s_addr);
        uint16_t port = ntohs(peer_addr.sin_port);
        snprintf(ip_str, sizeof(ip_str), "%d.%d.%d.%d", ip >> 24, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF);
        log_debug("AcceptNewSocket, socket=%d from %s:%d\n", fd, ip_str, port);

        pSocket->SetSocket(fd);
        pSocket->SetCallback(m_callback); // 第一次就是listenfd自己的m_callback, 后边会更改
        pSocket->SetCallbackData(m_callback_data);
        pSocket->SetState(SOCKET_STATE_CONNECTED); //都接受成功了，和connect() 不一样，所以直接SOCKET_STATE_CONNECTED
        pSocket->SetRemoteIP(ip_str);
        pSocket->SetRemotePort(port);

        
		_SetNoDelay(fd);		//禁止Nagle算法，会导致时延增加(数据小的时候不会立即发送，他希望多一点再发，所以有时延)，
		//Nagle算法就是为了尽可能发送大块数据，避免网络中充斥着许多小数据块。
		//更合理的方案还是应该使用一次大数据的写操作，而不是多次小数据的写操作
		//比如ssh就需要开，不然用户体验差
		//打开Nagel算法的好处，提高了网络利用率，但是这又不可避免地增加了延时
        _SetNonblock(fd);
        AddBaseSocket(pSocket);
        CEventDispatch::Instance()->AddEvent(fd, SOCKET_READ | SOCKET_EXCEP);
        //这里都是listenfd 的回调函数
        m_callback(m_callback_data, NETLIB_MSG_CONNECT, (net_handle_t)fd, NULL);
        
    }
}