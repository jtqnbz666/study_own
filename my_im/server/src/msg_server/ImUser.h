#ifndef IMUSER_H_
#define IMUSER_H_

#include "imconn.h"
#include "public_define.h"
#define MAX_ONLINE_FRIEND_CNT      100  //通知好友状态的最多个数

class CMsgConn;

class CImUser {
public:
    CImUser(string user_name);
    ~CImUser();
    
    void SetUserId(uint32_t user_id) { m_user_id = user_id; }
    uint32_t GetUserId() { return m_user_id; }
    string GetLoginName() { return m_login_name; }
    void SetNickName(string nick_name) { m_nick_name = nick_name; }
    string GetNickName() { return m_nick_name; }
    bool IsValidate() { return m_bValidate; }
    void SetValidated() { m_bValidate = true; }
    uint32_t GetPCLoginStatus() { return m_pc_login_status; }
    void SetPCLoginStatus(uint32_t pc_login_status) { m_pc_login_status = pc_login_status; }

    user_conn_t GetUserConn();
    bool IsMsgConnEmpty() { return m_conn_map.empty(); }
    void AddMsgConn(uint32_t handle, CMsgConn* pMsgConn) { m_conn_map[handle] = pMsgConn; }
    void DelMsgConn(uint32_t handle) { m_conn_map.erase(handle); }
    CMsgConn* GetMsgConn(uint32_t handle);
    void ValidateMsgConn(uint32_t handle, CMsgConn* pMsgConn); //验证连接的意思,就会把它加入map

    void AddUnValidateMsgConn(CMsgConn* pMsgConn) { m_unvalidate_conn_set.insert(pMsgConn); }//增加未验证的连接
    void DelUnValidateMsgConn(CMsgConn* pMsgConn) { m_unvalidate_conn_set.erase(pMsgConn); } //删除未验证的连接
    CMsgConn* GetUnValidateMsgConn(uint32_t handle);

    map<uint32_t, CMsgConn*>& GetMsgConnMap() { return m_conn_map; }
    void BroadcastPdu(CImPdu* pPdu, CMsgConn* pFromConn = NULL);
    void BroadcastPduWithOutMobile(CImPdu* pPdu, CMsgConn* pFromConn = NULL);
    void BroadcastPduToMobile(CImPdu* pPdu, CMsgConn* pFromConn = NULL);
    void BroadcastClientMsgData(CImPdu* pPdu, uint32_t msg_id, CMsgConn* pFromConn = NULL, uint32_t from_id = 0);
    void BroadcastData(void* buff, uint32_t len, CMsgConn* pFromConn = NULL);

    void HandleKickUser(CMsgConn* pConn, uint32_t reason);
    bool KickOutSameClientType(uint32_t client_type, uint32_t reason, CMsgConn* pFromConn = NULL);
    uint32_t GetClientTypeFlag();

private:
    uint32_t                m_user_id; //数据库生成的自增id
    string                  m_login_name;// qq号
    string                  m_nick_name; // 网名
    bool                    m_user_updated;
    uint32_t                m_pc_login_status; //在线或离线等状态
    bool                    m_bValidate;

    map<uint32_t, CMsgConn*>m_conn_map;     //fd 与 conn映射
    set<CMsgConn*>          m_unvalidate_conn_set;

};

typedef map<uint32_t, CImUser*> ImUserMap_t; //user_id, 与 CImUser 映射
typedef map<string, CImUser*>   ImUserMapByName_t;  //qq 与 CImUser映射

class CImUserManager {
public:
    CImUserManager() {}
    ~CImUserManager();

    static CImUserManager* GetInstance();
    CImUser* GetImUserById(uint32_t user_id);
    CImUser* GetImUserByLoginName(string login_name);
    CMsgConn* GetMsgConnByHandle(uint32_t user_id, uint32_t handle);
    bool AddImUserByLoginName(string login_name, CImUser* pUser);
    void RemoveImUserByLoginName(string login_name);

    bool AddImUserById(uint32_t user_id, CImUser* pUser);
    void RemoveImUserById(uint32_t user_id);

    void RemoveImUser(CImUser* pUser); //pUser->GetUserId();
    void RemoveAll();

    //状态，登陆端类型，user_id
    void GetOnlineUserInfo(list<user_stat_t>* online_user_info);

    //user_id , 个数(cnt)
    void GetUserConnCnt(list<user_conn_t>* user_conn_list, uint32_t& total_conn_cnt);

    void BroadcastPdu(CImPdu* pdu, uint32_t client_type_flag);

private:
    ImUserMap_t         m_im_user_map;  //user_id, 与 CImUser 映射
    ImUserMapByName_t   m_im_user_map_by_name; // qq 与 CImUser 映射
};

void get_online_user_info(list<user_stat_t>* online_user_info);

#endif