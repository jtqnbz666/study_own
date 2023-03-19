#ifndef __CACHEMANAGER_H__
#define __CACHEMANAGER_H__

#include <list>
#include <map>
#include "ostype.h"
#include "Lock.h"
#include "Condition.h"
#include "ImPduBase.h"
#include "public_define.h"
#include "IM.BaseDefine.pb.h"

class CSyncCenter {
public:
    static CSyncCenter* getInstance();
    uint32_t getLastUpdate() {
        CAutoLock auto_lock(&last_update_lock_);
        return m_nLastUpdate;
    }
    uint32_t getLastUpdateGroup() {
        CAutoLock auto_lock(&last_update_lock_);
        return m_nLastUpdateGroup;
    }
    string getDeptName(uint32_t nDeptId);   //获取部门名称
    void startSync();                       //开始同步群组信息，IMGroup表同步到，将信息同步到IMRecentSession表中表
    void stopSync();
    void init();
    void updateTotalUpdate(uint32_t nUpdated);

private:
    void updateLastUpdateGroup(uint32_t nUpdated);
    CSyncCenter();
    ~CSyncCenter();
    static void* doSyncGroupChat(void* arg);

private:
    void getDept(uint32_t nDeptId, DBDeptInfo_t** pDept);
    DBDeptMap_t* m_pDeptInfo;    //部门id 与 部门信息的对应， 不是群组

    static CSyncCenter* m_pInstance;
    uint32_t            m_nLastUpdateGroup;
    uint32_t            m_nLastUpdate;

    CCondition*         m_pCondGroupChat;
    CLock*              m_pLockGroupChat;
    static bool         m_bSyncGroupChatRuning;
    bool                m_bSyncGroupChatWaitting;  

    pthread_t           m_nGroupChatThreadId; //因为这个同步过程是开的一个线程来执行的
    CLock               last_update_lock_;
};
#endif