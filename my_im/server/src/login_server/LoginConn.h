#ifndef LOGINCONN_H_
#define LOGINCONN_H_

#include "imconn.h"

//不管是client 还是 msg 的连接， 都是用的LoginConn对象， 没有单独划分为MsgConn, 和 ClientConn

enum {
    LOGIN_CONN_TYPE_CLIENT = 1,
    LOGIN_CONN_TYPE_MSG_SERV,
};

//因为loginserver负责给客户分发msgserver，所以需要保存msgserver信息
typedef struct {
    string      ip_addr1; //电信IP
    string      ip_addr2; //网通IP
    uint16_t    port;
    uint32_t    max_conn_cnt;
    uint32_t    cur_conn_cnt;
    string      hostname;   //消息服务器的主机名
} msg_serv_info_t;

class CLoginConn : public CImConn {
public: 
    CLoginConn();
    virtual ~CLoginConn();
    virtual void Close();
    void OnConnect2(net_handle_t handle, int conn_type);
    virtual void OnClose();
    virtual void OnTimer(uint64_t curr_tick);   //心跳
    virtual void HandlePdu(CImPdu* pPdu);

private:
    //对不同数据的处理
    void _HandleMsgServInfo(CImPdu* pPdu);      //接收msgserver
    void _HandleUserCntUpdate(CImPdu* pPdu);    //msgserver通知用户人员变化
    void _HandleMsgServRequest(CImPdu* pPdu);   //client请求分配msgserver

private:
    int m_conn_type;
};

void init_login_conn();  //通过netlib_register_timer注册定时器处理心跳
#endif 