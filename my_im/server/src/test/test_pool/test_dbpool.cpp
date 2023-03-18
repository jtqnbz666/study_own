#include <iostream>
#include "DBPool.h"
#include "CachePool.h"
#include "ConfigFileReader.h"


static CThreadPool g_thread_pool;
int main() {
    CDBManager* pDBManager = CDBManager::getInstance();
    CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_slave");
    //需要libbase.a  libmysqlclient.a  libslog.so   pthread
    if(pDBConn) {
        printf("DBConn-yes\n");
    } else {
        printf("errno");
    }


    cout << "-----------------------------" << endl;
    CacheManager* pCacheManager = CacheManager::getInstance();
    CacheConn* pCacheConn = pCacheManager->GetCacheConn("token");
    //需要libhiredis.a  , hiredis.h, read.h, sds.h
    if(pCacheConn) {
        printf("CacheConn-yes\n");
    } else {
        printf("errno");
    }

    cout << "-----------------------------" << endl;
   
    if(g_thread_pool.Init(8) != 0) log("error threadpool_init!");
    return 0;
}
