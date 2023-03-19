
#ifndef MESSAGECOUNTER_H_
#define MESSAGECOUNTER_H_

#include "ImPduBase.h"
namespace DB_PROXY {

    void getUnreadMsgCounter(CImPdu* pPdu, uint32_t conn_uuid);
    void clearUnreadMsgCounter(CImPdu* pPdu, uint32_t conn_uuid);
    
    void setDevicesToken(CImPdu* pPdu, uint32_t conn_uuid);
    void getDevicesToken(CImPdu* pPdu, uint32_t conn_uuid);
};


#endif /* MESSAGECOUNTER_H_ */
