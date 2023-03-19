#ifndef HANDLERMAP_H_
#define HANDLERMAP_H_

#include "../base/util.h"
#include "ProxyTask.h"
//以命令来映射对应的处理函数
typedef map<uint32_t, pdu_handler_t> HandlerMap_t;
class CHandlerMap {
public:
    virtual ~CHandlerMap();
    static CHandlerMap* getInstance();
    void Init();
    pdu_handler_t GetHandler(uint32_t pdu_type);

private:
    CHandlerMap();

private:
    static CHandlerMap* s_handler_instance;
    HandlerMap_t    m_handler_map;
};
#endif