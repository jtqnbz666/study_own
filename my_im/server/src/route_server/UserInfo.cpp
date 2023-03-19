#include "UserInfo.h"
#include "ImPduBase.h"
#include "public_define.h"
#include "IM.BaseDefine.pb.h"
using namespace IM::BaseDefine;
CUserInfo::CUserInfo() {

}

CUserInfo::~CUserInfo() {

}

void CUserInfo::AddClientType(uint32_t client_type) {
    map<uint32_t, uint32_t>::iterator it = m_ClientTypeList.find(client_type);
    CDBManager* pDBManager = CDBManager::getInstance();
    CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_slave");
    
    if(it != m_ClientTypeList.end()) {
        it->second += 1;
    } else {
        m_ClientTypeList[client_type] = 1;
        if(pDBConn) {
        string strSql = "update IMUser_State set state = 1 where user_id = " + int2string(m_user_id);
        CResultSet* pResult = pDBConn->ExecuteQuery(strSql.c_str());
        pDBManager->RelDBConn(pDBConn);
        }
    }
}

void CUserInfo::RemoveClientType(uint32_t client_type) {
    map<uint32_t, uint32_t>::iterator it = m_ClientTypeList.find(client_type);
    if(it != m_ClientTypeList.end()) {
        uint32_t count = it->second;
        count -= 1;
        if(count > 0) {
            it->second = count;
        } else {
            CDBManager* pDBManager = CDBManager::getInstance();
            CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_slave");
            if(pDBConn) {
                string strSql = "update IMUser_State set state = 2 where user_id = " + int2string(m_user_id);
                CResultSet* pResult = pDBConn->ExecuteQuery(strSql.c_str());
                pDBManager->RelDBConn(pDBConn);
            }
            m_ClientTypeList.erase(client_type);
        }
    }
}

bool CUserInfo::FindRouteConn(CRouteConn* pConn) {
    set<CRouteConn*>::iterator it = m_RouteConnSet.find(pConn);
    if(it != m_RouteConnSet.end()) {
        return true;
    } else {
        return false;
    }
}

uint32_t CUserInfo::GetCountByClientType(uint32_t client_type) {
    map<uint32_t, uint32_t>::iterator it = m_ClientTypeList.find(client_type);
    if(it != m_ClientTypeList.end()) {
        return it->second;
    } else {
        return 0;
    }
}

bool CUserInfo::IsMsgConnNULL() {
    if(m_ClientTypeList.size() == 0) {
        return true;
    } else {
        return false;
    }
}

void CUserInfo::ClearClientType() {
    m_ClientTypeList.clear();
}

bool CUserInfo::IsPCClientLogin() {
    bool bRet = false;
    map<uint32_t, uint32_t>::iterator it = m_ClientTypeList.begin();
    for(; it != m_ClientTypeList.end(); it ++) {
        uint32_t client_type = it->first;
        if(CHECK_CLIENT_TYPE_PC(client_type)) { //检查是否是pc端
            bRet = true;
            break;
        }
    }
    return bRet;
}

bool CUserInfo::IsMobileClientLogin() {
    bool bRet = false;
    map<uint32_t, uint32_t>::iterator it = m_ClientTypeList.begin();
    for(; it != m_ClientTypeList.end(); it ++) {
        uint32_t client_type = it->first;
        if(CHECK_CLIENT_TYPE_MOBILE(client_type)) {
            bRet = true;
            break;
        }
    }
    return bRet;
}

uint32_t CUserInfo::GetStatus() {  //检查是否PC端在线
    uint32_t status = USER_STATUS_OFFLINE;
    map<uint32_t, uint32_t>::iterator it = m_ClientTypeList.begin();
    for(; it != m_ClientTypeList.end(); it ++) {
        uint32_t client_type = it->first;
        if(CHECK_CLIENT_TYPE_PC(client_type)) {
            status = USER_STATUS_ONLINE;
            break;
        }
    }
    return status;
}