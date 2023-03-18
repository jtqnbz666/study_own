#ifndef CACHEPOOL_H_
#define CACHEPOOL_H_

#include <vector>
#include "../base/util.h"
#include "ThreadPool.h"
#include "hiredis.h"

class CachePool;

class CacheConn {
    public:
    CacheConn(CachePool* pCachePool);
    virtual ~CacheConn();

    int Init();
    const char* GetPoolName();

    string get(string key);
    string setex(string key, int timeout, string value);
    string set(string key, string& value);

    //批量获取
    bool mget(const vector<string>& keys, map<string, string>&ret_value);
    //判断一个key是否存在
    bool isExists(string& key);

    //Redis hash structure
    long hdel(string key, string field);
    string hget(string key, string field);
    bool hgetAll(string key, map<string, string>& ret_value);
    long hset(string key, string field, string value);

    long hincrBy(string key, string field, long value);
    long incrBy(string key, long value);
    string hmset(string key, map<string, string>& hash);
    bool hmget(string key, list<string>& fields, list<string>& ret_value);

    //原子加减1
    long incr(string key);
    long decr(string key);

    //Redis list structure
    long lpush(string key, string value);
    long rpush(string key, string value);
    long llen(string key);
    bool lrange(string key, long start, long end, list<string>& ret_value);

private:
    CachePool*      m_pCachePool;
    redisContext*   m_pContext; // 用到了hiredis; 上下文，一般在执行语句的时候都需要传入
    uint64_t        m_last_connect_time;
};

class CachePool {
public:
    CachePool(const char* pool_name, const char* server_ip, int server_port, int db_name, int max_conn_cnt);
    virtual ~CachePool();
    
    int Init();
    CacheConn* GetCacheConn();
    void RelCacheConn(CacheConn* pCacheConn);

    const char* GetPoolName() { return m_pool_name.c_str(); }
    const char* GetServerIP() { return m_server_ip.c_str(); }
    int GetServerPort() { return m_server_port; }
    int GetDBNum() { return m_db_num; }
private:
    string        m_pool_name;
    string        m_server_ip;
    int           m_server_port;
    int           m_db_num;

    int           m_cur_conn_cnt;
    int           m_max_conn_cnt;
    list<CacheConn*>        m_free_list;
    CThreadNotify           m_free_notify;
};

class CacheManager {
public:
    virtual ~CacheManager();
    static CacheManager* getInstance();
    int Init();
    CacheConn* GetCacheConn(const char* pool_name);
    void RelCacheConn(CacheConn* pCacheConn);
private:
    CacheManager();

private:
    static CacheManager*    s_cache_manager;
    map<string, CachePool*> m_cache_pool_map;
};

#endif