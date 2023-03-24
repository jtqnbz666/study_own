#include "imconn.h"

static CImConn* FindImConn(ConnMap_t* imconn_map, net_handle_t handle) {
    CImConn* pConn = NULL;
    ConnMap_t::iterator iter = imconn_map->find(handle);
    if(iter != imconn_map->end()) {
        pConn = iter->second;
        pConn->AddRef();    //增加一次引用
    }
    return pConn;
}
//传过来的callback_data是一个hash-map
//而在httpconn中的httpconn_callback传过来的是conn_handle(不是fd)，hash表的key
//为什么这里传hash-map而httpconn传conn_handle，因为imconn是基类，不传hash-map就找不到对应关系
//而httpconn中，可以理解为既是基类又是子类，hash-map就在httpconn文件中。
void imconn_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam) {
    NOTUSED_ARG(handle);
    NOTUSED_ARG(pParam);
    if(!callback_data) return;
    ConnMap_t* conn_map = (ConnMap_t*)callback_data;// 从这里就可以看出来只是一张hash表
    CImConn* pConn = FindImConn(conn_map, handle);
    if(!pConn) return ;
    switch(msg) {
    case NETLIB_MSG_CONFIRM:
        pConn->OnConfirm();
        break;
    case NETLIB_MSG_READ:
        pConn->OnRead();
        break;
    case NETLIB_MSG_WRITE:
        pConn->OnWrite();
        break;
    case NETLIB_MSG_CLOSE:
        pConn->OnClose();
        break;
    default:
        log_error("!!!imconn_callback error msg: %d", msg);
        break;
    }
    pConn->ReleaseRef(); //引用减一
}

CImConn::CImConn() {
    log_debug("CImConn::CImConn");
    m_busy = false;
    m_handle = NETLIB_INVALID_HANDLE;
    m_recv_bytes = 0;
    m_last_send_tick = m_last_recv_tick = get_tick_count();
}

CImConn::~CImConn() {
    log_debug("CImConn::~CImConn, handle = %d ", m_handle);
}

int CImConn::Send(void* data, int len) {
    //因为发送数据了，所以更新， 发送心跳包也是根据它设计的
    m_last_send_tick = get_tick_count();
    if(m_busy) {
        m_out_buf.Write(data, len); //如果在忙，就直接写进m_out_buf
        return len;
    }
    int offset = 0;     //因为一次不一定发的完
    int remain = len;
    while(remain > 0) {
        int send_size = remain;
        if(send_size > NETLIB_MAX_SOCKET_BUF_SIZE) {
            send_size = NETLIB_MAX_SOCKET_BUF_SIZE;
        }
        int ret = netlib_send(m_handle, (char*)data + offset, send_size);
        if(ret <= 0) {
            ret = 0;
            break; //如果网络等原因，没有发送成功，则退出循环
        }
        offset += ret;
        remain -= ret;
    }
    if(remain > 0) {
        m_out_buf.Write((char*)data + offset, remain);  //如果网络等原因没有发送完的数据，就写到这里
        m_busy = true;
        log_debug("send busy, remain=%d", m_out_buf.GetWriteOffset());
    } else {
        OnWriteCompelete();//这里没有做任何事,可以记录一下日志
    }
    return len;
}

void CImConn::OnRead() {
    for(; ;) {
        uint32_t free_buf_len = m_in_buf.GetAllocSize() - m_in_buf.GetWriteOffset();
        if(free_buf_len < READ_BUF_SIZE) 
            m_in_buf.Extend(READ_BUF_SIZE);//空闲空间太少了还是需要扩容
        log_debug("handle=%u, netlib_recv into, time=%u\n", m_handle, get_tick_count());
        
        //注意这个细节， 读取的时候是加了偏移量的
        int ret = netlib_recv(m_handle, m_in_buf.GetBuffer() + m_in_buf.GetWriteOffset(), READ_BUF_SIZE);
        if(ret <= 0) break;
        
        m_recv_bytes += ret;
        m_in_buf.IncWriteOffset(ret); //记录大小

        m_last_recv_tick = get_tick_count();
    }
    CImPdu* pPdu = NULL;
    try {
        while( (pPdu = CImPdu::ReadPdu(m_in_buf.GetBuffer(), m_in_buf.GetWriteOffset()))) {
            uint32_t pdu_len = pPdu->GetLength();
            log_debug("handle = %u, pdu_len into = %u\n", m_handle, pdu_len);
            HandlePdu(pPdu);
            m_in_buf.Read(NULL, pdu_len); //在上一步处理了pPdu后需要清理读取的数据，不一定是全部数据
            delete pPdu;
            pPdu = NULL;
        } 
    } catch (CPduException& ex) {
        log_error("!!!catch exception, sid=%u, cid=%u, err_code=%u, err_msg=%s, close the connection ",
				ex.GetServiceId(), ex.GetCommandId(), ex.GetErrorCode(), ex.GetErrorMsg());
        if(pPdu) {
            delete pPdu;
            pPdu = NULL;
        } 
        OnClose();
    }
}

void CImConn::OnWrite() { //按理来说正常发数据是在Send中,但如果有没发送的数据,当写时间触发时完成
    if(!m_busy) return; //!m_busy 则表示没有数据在m_out_buf
    while(m_out_buf.GetWriteOffset() > 0) {
        int send_size = m_out_buf.GetWriteOffset();
        if(send_size > NETLIB_MAX_SOCKET_BUF_SIZE) { //128k,单次只能发这么多
            send_size = NETLIB_MAX_SOCKET_BUF_SIZE;
        }
        int ret = netlib_send(m_handle, m_out_buf.GetBuffer(), send_size);
        if(ret <= 0) {
            ret = 0;
            break;          //出错了就退出，比如网络繁忙
        }
        m_out_buf.Read(NULL, ret); //读多少，就清理多少,memmove
    }
    if(m_out_buf.GetWriteOffset() == 0) {
        m_busy = false;
    }
    log_debug("onWrite, remain=%d", m_out_buf.GetWriteOffset());
}