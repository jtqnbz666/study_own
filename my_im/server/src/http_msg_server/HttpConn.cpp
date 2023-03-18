#include "HttpConn.h"
#include "HttpParserWrapper.h"
#include "HttpQuery.h"

static HttpConnMap_t g_http_conn_map; 
//一个递增的数字g_conn_handle_generator(不是fd)  和 httpconn映射


//conn_handle 从0开始递增， 可以防止因socket handle重用引起的一些冲突
static uint32_t g_conn_handle_generator = 0;

CHttpConn* FindHttpConnByHandle(uint32_t conn_handle) {
    CHttpConn* pConn = NULL;
    HttpConnMap_t::iterator it = g_http_conn_map.find(conn_handle);
    if(it != g_http_conn_map.end()) {
        pConn = it->second;
    }
    return pConn;
}

//对应ImConn中的imconn_callback
void httpconn_callback(void* callback_data, uint8_t msg, uint32_t handle, uint32_t uParam, void* pParam) {
    NOTUSED_ARG(uParam);
    NOTUSED_ARG(pParam); 
    
    //对于imconn_callback的callback_data就是一个hash表， 
    //但httpconn_callback的callback_data是con_handle(一个递增数字不是handle)
    //因为不管是哪个callback都是与外界环境隔离的，所以callback_data是必须的
    uint32_t conn_handle = *((uint32_t*)(&callback_data));//是hash表的key，但不是fd
    CHttpConn* pConn = FindHttpConnByHandle(conn_handle);
    if(!pConn) return ;
    switch(msg) {
    case NETLIB_MSG_READ:
        pConn->OnRead();
        break;
    case NETLIB_MSG_WRITE:
        pConn->OnWrite();
        break;
    default:
        log_error("!!!httpconn_callback error msg:%d", msg);
        break;
    }
}

//ImConn中是没有这个的，这个相当于类似LoginConn或者MsgConn等继承下来的对象中才有；
//在初始化的时候注册为定时器
void http_conn_timer_callback(void* call_data, uint8_t msg, uint32_t handle, void* pParam) {
    CHttpConn* pConn = NULL;
    HttpConnMap_t::iterator it, it_old;
    uint64_t cur_time = get_tick_count();

    for(it = g_http_conn_map.begin(); it != g_http_conn_map.end(); ) {
        it_old = it;
        it ++;
        pConn = it_old->second;
        pConn->OnTimer(cur_time);
    }

}

void init_http_conn() {
    netlib_register_timer(http_conn_timer_callback, NULL, 1000);
}

CHttpConn::CHttpConn() {
    m_busy = false;
    m_sock_handle = NETLIB_INVALID_HANDLE;
    m_state = CONN_STATE_IDLE;
    m_last_send_tick = m_last_recv_tick = get_tick_count();
    m_conn_handle = ++g_conn_handle_generator;
    if(m_conn_handle == 0) {
        m_conn_handle = ++g_conn_handle_generator;
    }
}

CHttpConn::~CHttpConn() {
    log("~CHttpConn, handle=%u\n", m_conn_handle);
}

int CHttpConn::Send(void* data, int len) {
    m_last_send_tick = get_tick_count();
    if(m_busy) {
        m_out_buf.Write(data, len);
        return len;
    }
    
    int ret = netlib_send(m_sock_handle, data, len);
    if(ret < 0) ret = 0;
    if(ret < len) {
        m_out_buf.Write((char*)data + ret, len - ret);
        m_busy = true;
        //log("not send all, remain=%d\n", m_out_buf.GetWriteOffset());
    } else {
        OnWriteComplete();
    }
    return len;
}

void CHttpConn::Close() {
    m_state = CONN_STATE_CLOSED;
    g_http_conn_map.erase(m_conn_handle);
    netlib_close(m_sock_handle);
    ReleaseRef();
}

void CHttpConn::OnConnect(net_handle_t handle) {
    log("OnConnect, handle=%d\n", handle);
    m_sock_handle = handle;
    m_state = CONN_STATE_CONNECTED;
    g_http_conn_map.insert(make_pair(m_conn_handle, this));
    netlib_option(handle, NETLIB_OPT_SET_CALLBACK, (void*)httpconn_callback);
    netlib_option(handle, NETLIB_OPT_SET_CALLBACK_DATA, reinterpret_cast<void*>(m_conn_handle));
    netlib_option(handle, NETLIB_OPT_GET_REMOTE_IP, (void*)&m_peer_ip);
}

void CHttpConn::OnRead() {
    for(;;) {
        uint32_t free_buf_len = m_in_buf.GetAllocSize() - m_in_buf.GetWriteOffset();
        if(free_buf_len < READ_BUF_SIZE + 1) 
            m_in_buf.Extend(READ_BUF_SIZE + 1);
        int ret = netlib_recv(m_sock_handle, m_in_buf.GetBuffer() + m_in_buf.GetWriteOffset(), READ_BUF_SIZE);
        if(ret <= 0) break;
        m_in_buf.IncWriteOffset(ret);
        m_last_recv_tick = get_tick_count();
    }

    //每次请求对应一个HTTP连接， 所以读完数据后， 不用在同一个连接里面准备读取下个请求
    char* in_buf = (char*)m_in_buf.GetBuffer();
    uint32_t buf_len = m_in_buf.GetWriteOffset();
    in_buf[buf_len] = '\0';

    //如果buf_len 过长则可能是收到攻击, 则断开连接
    //正常的url长度是2048,我们接收的所有数据长度不得大于1K
    if(buf_len < 1024) {
        log_error("get too much data:%s", in_buf);
        Close();
        return ;
    }
    //log("OnRead, buf_len=%u, conn_handle=%u\n", buf_len, m_conn_handle);

    //解析http数据
    m_cHttpParser.ParseHttpContent(in_buf, buf_len);
    if(m_cHttpParser.IsReadAll()) {
        string url = m_cHttpParser.GetUrl();
        if(strncmp(url.c_str(), "/query/", 7) == 0) { //路由判断
            string content = m_cHttpParser.GetBodyContent();
            CHttpQuery* pQueryInstance = CHttpQuery::GetInstance();
			pQueryInstance->DispatchQuery(url, content, this);
        } else {
            log_error("url unknown, url=%s",url.c_str());
            Close();
        }
    }
}

void CHttpConn::OnWrite() {
    if(!m_busy) return;

    //imconn中是循环写
    int ret = netlib_send(m_sock_handle, m_out_buf.GetBuffer(), m_out_buf.GetWriteOffset());
    if( ret < 0 ) ret = 0;
    int out_buf_size = (int)m_out_buf.GetWriteOffset();
    m_out_buf.Read(NULL, ret);
    if(ret < out_buf_size) {    //表明没有发完
        m_busy = true;
        log_debug("not send all, remain=%d", m_out_buf.GetWriteOffset());
    } else {
        OnWriteComplete();
        m_busy = false;
    }
}

void CHttpConn::OnClose() {
    Close();
}

void CHttpConn::OnTimer(uint64_t curr_tick) {
    if(curr_tick > m_last_recv_tick + HTTP_CONN_TIMEOUT) { //1min
        log_warn("HttpConn timeout, handle=%d", m_conn_handle);
        Close();
    }
}

void CHttpConn::OnWriteComplete() {
    log("write complete") ;
    Close();
}