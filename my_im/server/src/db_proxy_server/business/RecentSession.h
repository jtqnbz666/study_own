
#ifndef FRIEND_SHIP_H_
#define FRIEND_SHIP_H_

#include "ImPduBase.h"

namespace DB_PROXY {

    void getRecentSession(CImPdu* pPdu, uint32_t conn_uuid);
    
    void deleteRecentSession(CImPdu* pPdu, uint32_t conn_uuid);

};


#endif /* FRIEND_SHIP_H_ */
