#ifndef ROUTECONN_H_
#define ROUTECONN_H_

#include "imconn.h"
#include "../db_proxy_server/DBPool.h"
#include "../db_proxy_server/CachePool.h"

class CRouteConn : public CImConn {
public:
    CRouteConn();
    virtual ~CRouteConn();
    virtual void Close();
    virtual void OnConnect(net_handle_t handle);
    virtual void OnClose();
    virtual void OnTimer(uint64_t curr_tick);
    virtual void HandlePdu(CImPdu* pPdu);

private:
    void _HandleOnlineUserInfo(CImPdu* pPdu);
    void _HandleUserStatusUpdate(CImPdu* pPdu);
    void _HandleRoleSet(CImPdu* pPdu);
    void _HandleUsersStatusRequest(CImPdu* pPdu);
    void _HandleMsgReadNotify(CImPdu* pPdu);
    void _HandleKickUser(CImPdu* pPdu);

    void _DispatchFriend(uint32_t frient_cnt, uint32_t* frient_id_list);

    //将PDU转发到除当前的其他msgserv上
    void _BroadcastMsg(CImPdu* pPdu, CRouteConn* pFromConn = NULL);

private:
    void _UpdateUserStatus(uint32_t user_id, uint32_t status, uint32_t client_type);

    //指定将pdu发给某个user_id;
    void _SendPduToUser(uint32_t user_id, CImPdu* pPdu, bool bAll);

private:
    bool            m_bMaster;
};

void init_routeconn_timer_callback();

#endif