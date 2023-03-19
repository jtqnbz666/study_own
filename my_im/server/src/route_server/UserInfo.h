#ifndef __im_server_TT__UserInfo__
#define __im_server_TT__UserInfo__

#include <iostream>
#include <set>
#include <map>
#include "ostype.h"
#include "../db_proxy_server/DBPool.h"
class CRouteConn;   //前置声明，引用别的文件的类，函数也是这样处理的

using namespace std;

class CUserInfo {
public:
    CUserInfo();
    ~CUserInfo();

    uint32_t  GetStatus();
    void AddRouteConn(CRouteConn* pConn) { m_RouteConnSet.insert(pConn); }
    void RemoveRouteConn(CRouteConn* pConn) { m_RouteConnSet.erase(pConn); }
    void ClearRouteConn() { m_RouteConnSet.clear(); }
    set<CRouteConn*>* GetRouteConn() { return &m_RouteConnSet; }
    bool FindRouteConn(CRouteConn* pConn);

    uint32_t GetRouteConnCount() { return m_RouteConnSet.size(); }
    void AddClientType(uint32_t client_type); 
    void RemoveClientType(uint32_t client_type);
    uint32_t GetCountByClientType(uint32_t client_type);
    bool IsMsgConnNULL();
    void ClearClientType();

    bool IsPCClientLogin();
    bool IsMobileClientLogin();
    uint32_t m_user_id;

private:
    set<CRouteConn*> m_RouteConnSet;    //user_id 与 对应所在的msgserv的集合
    map<uint32_t/*client_type*/, uint32_t/*count*/>m_ClientTypeList;
};
#endif