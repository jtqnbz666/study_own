#include "LoginConn.h"//用来处理client和msgserv连接请求
#include "netlib.h"
#include "ConfigFileReader.h"
#include "version.h"
#include "HttpConn.h" //用来处理http请求
#include "ipparser.h"

IpParser* pIpParser = NULL;
string strMsfsUrl;
string strDiscovery; //发现获取地址


//client客户端连接请求(不是http请求)
void client_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam) {
    if(msg == NETLIB_MSG_CONNECT) {
        CLoginConn* pConn = new CLoginConn();

        //这个OnConnect和别的conn不同的是它负责处理client和msgserver两种请求
        pConn->OnConnect2(handle, LOGIN_CONN_TYPE_CLIENT);//这个handle对于listenfd来讲是客户端的fd
    } else {
        log_error("!!!error msg:%d", msg);
    }
}

//msg_server连接事件
void msg_serv_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam) {
    log("msg_server come in");
    if(msg == NETLIB_MSG_CONNECT) {
        CLoginConn* pConn = new CLoginConn();
        pConn->OnConnect2(handle, LOGIN_CONN_TYPE_MSG_SERV);
    } else {
        log_error("!!!error msg: %d", msg);
    }
}

//Android,IOS,PC等客户端请求连接事件, http请求
void http_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam) {
    if(msg == NETLIB_MSG_CONNECT) {
        //为什么new了没有释放,
        //conn对象都有一个refcount计数,当为0是才会delete.
        CHttpConn* pConn = new CHttpConn();
        pConn->OnConnect(handle);
    } else {
        log_error("!!!error msg:%d", msg);
    }
}

int main(int argc, char* argv[]) {
    if((argc == 2) && (strcmp(argv[1], "-v") == 0)) {
        log_fatal("Server Version:LoginServer%s\n", VERSION);//fatal 致命的
        log_fatal("Server Build:%s %s\n", __DATE__, __TIME__);
        return 0;
    }
    signal(SIGPIPE, SIG_IGN);
    CConfigFileReader config_file("loginserver.conf");
    char* client_listen_ip = config_file.GetConfigName("ClientListenIP");
    char* str_client_port = config_file.GetConfigName("ClientPort");
    char* http_listen_ip = config_file.GetConfigName("HttpListenIP");
    char* str_http_port = config_file.GetConfigName("HttpPort");
    char* msg_server_listen_ip = config_file.GetConfigName("MsgServerListenIP");
    char* str_msg_server_port = config_file.GetConfigName("MsgServerPort");
    char* str_msfs_url = config_file.GetConfigName("msfs");
    char* str_discovery = config_file.GetConfigName("discovery");

    if(!msg_server_listen_ip || !str_msg_server_port || !http_listen_ip || 
        ! str_http_port || !str_msfs_url || !str_discovery) {
            log("config item missing, exit...");
            return -1;
    }
    uint16_t client_port = atoi(str_client_port);
    uint16_t msg_server_port = atoi(str_msg_server_port);
    uint16_t http_port = atoi(str_http_port);
    strMsfsUrl = str_msfs_url;
    strDiscovery = str_discovery;

    pIpParser = new IpParser();
    int ret = netlib_init();

    if(ret == NETLIB_ERROR) return ret;
    CStrExplode client_listen_ip_list(client_listen_ip, ';');
    for(uint32_t i = 0; i < client_listen_ip_list.GetItemCnt(); i ++) {
        ret = netlib_listen(client_listen_ip_list.GetItem(i), client_port, client_callback, NULL);
        if(ret == NETLIB_ERROR) return ret;
    }
    CStrExplode msg_server_listen_ip_list(msg_server_listen_ip, ';');
    for(uint32_t i = 0; i < msg_server_listen_ip_list.GetItemCnt(); i ++) {
        ret = netlib_listen(msg_server_listen_ip_list.GetItem(i), msg_server_port, msg_serv_callback, NULL);
        if(ret == NETLIB_ERROR) return ret;
    }
    CStrExplode http_listen_ip_list(http_listen_ip, ';');
    for(uint32_t i = 0; i < http_listen_ip_list.GetItemCnt(); i ++) {
        ret = netlib_listen(http_listen_ip_list.GetItem(i), http_port, http_callback, NULL);
        if(ret == NETLIB_ERROR) return ret;
    }

    log("server start listen on:\n For client %s:%d\nFor MsgServer:%s:%d\n For http:%s:%d\n",
        client_listen_ip, client_port, msg_server_listen_ip, msg_server_port,http_listen_ip, http_port);
    init_login_conn();  //注册定时器 , 处理连接
    init_http_conn(); //注册定时器, 处理连接
    log("now enter the event loop...\n");
    writePid();
    netlib_eventloop();

    return 0;
} 
