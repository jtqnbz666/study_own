#ifndef IMCONN_H_
#define IMCONN_H_
#include "netlib.h"
#include "util.h"
#include "ImPduBase.h"

#define SERVER_HEARTBEAT_INTERVAL   5000
#define SERVER_TIMEOUT              30000
#define CLIENT_HEARTBEAT_INTERVAL   30000
#define CLIENT_TIMEOUT              120000
#define MOBILE_CLIENT_TIMEOUT       60000 * 5
#define READ_BUF_SIZE               2048

class CImConn : public CRefObject { //继承它的原因都是为了引用计数， 确保没有使用了再释放资源
public:
    CImConn();
    virtual ~CImConn();

    bool IsBusy() { return m_busy; }
    int SendPdu(CImPdu* pPdu) { return Send(pPdu->GetBuffer(), pPdu->GetLength()); }
    int Send(void* data, int len);

    virtual void OnConnect(net_handle_t handle) { m_handle = handle; }
    virtual void OnConfirm() {}
    virtual void OnRead();
    virtual void OnWrite();
    virtual void OnClose() {}
    virtual void OnTimer(uint64_t curr_tick) {} //心跳包设计
    virtual void OnWriteCompelete() {}
    virtual void HandlePdu(CImPdu* pPdu) {}

protected:
    net_handle_t        m_handle;
    bool                m_busy;

    string              m_peer_ip;
    uint16_t            m_peer_port;
    CSimpleBuffer       m_in_buf;       //读写缓冲区分开
    CSimpleBuffer       m_out_buf;

    bool                m_policy_conn;
    uint32_t            m_recv_bytes;
    uint64_t            m_last_send_tick;      //用于心跳设计
    uint64_t            m_last_recv_tick; 
    uint64_t            m_last_all_user_tick;
};

typedef hash_map<net_handle_t, CImConn*> ConnMap_t; // fd 与 conn
typedef hash_map<uint32_t, CImConn*> UserMap_t;  //uuid 与 conn在 db_proxy_Server中有用

void imconn_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam);
void ReadPolicyFile();

#endif