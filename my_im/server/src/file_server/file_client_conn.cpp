#include "file_client_conn.h"
#include "IM.Other.pb.h"
#include "IM.File.pb.h"
#include "im_conn_util.h"
#include "config_util.h"
#include "transfer_task_manager.h"

using namespace IM::BaseDefine;
static ConnMap_t g_file_client_conn_map;//client_fd 与 conn

void FileClientConnCallback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam) {
    if(msg == NETLIB_MSG_CONNECT) {
        FileClientConn* conn = new FileClientConn();
        conn->OnConnect(handle);
    } else {
        log("error msg:%d", msg);
    }
}

//心跳连接，每一个继承自imconn的conn对象都有
void FileClientConnTimerCallback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam) {
    uint64_t cur_time = get_tick_count();
    for(ConnMap_t::iterator it = g_file_client_conn_map.begin(); it != g_file_client_conn_map.end(); ) {
        ConnMap_t::iterator it_old = it;
        it ++;

        FileClientConn* conn = (FileClientConn*)it_old->second;
        conn->OnTimer(cur_time);
    }
}

void FileTaskTimerCallback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam) {
    uint64_t tick = get_tick_count();
    TransferTaskManager::GetInstance()->OnTimer(tick);
}

//与其他服务器不同的是，它需要初始化两个定时器，一个用于心跳，一个用于检查任务
void InitializeFileClientConn() {
    netlib_register_timer(FileClientConnTimerCallback, NULL, 1000);
    netlib_register_timer(FileTaskTimerCallback, NULL, 10000);
}

void FileClientConn::Close() {
    log("close client, handle %d", m_handle);
    if(transfer_task_) {
        if(transfer_task_->GetTransMode() == FILE_TYPE_ONLINE) {    //在线传输
            transfer_task_->set_state(kTransferTaskStateInvalid); //非法
        } else {    //离线传输
            if(transfer_task_->state() >= kTransferTaskStateUploadEnd) { //上传完毕之后的状态(等待下载)
                transfer_task_->set_state(kTransferTaskStateWaitingDownload); //等待下载
            }
        }
        transfer_task_->SetConnByUserID(user_id_, nullptr);//断开conn的绑定

        //对于离线传输， 如果是等待上传状态就不会删除任务
        TransferTaskManager::GetInstance()->DeleteTransferTaskByConnClose(transfer_task_->task_id());
        transfer_task_ =  nullptr;
    }
    auth_ = false;
    if(m_handle != NETLIB_INVALID_HANDLE) {
        netlib_close(m_handle);
        g_file_client_conn_map.erase(m_handle);
    }
    ReleaseRef();
}

void FileClientConn::OnConnect(net_handle_t handle) {
    m_handle = handle;
    g_file_client_conn_map.insert(make_pair(handle, this));
    netlib_option(handle, NETLIB_OPT_SET_CALLBACK, (void*)imconn_callback);
    netlib_option(handle, NETLIB_OPT_SET_CALLBACK_DATA, (void*)&g_file_client_conn_map);

    uint32_t socket_buf_size = NETLIB_MAX_SOCKET_BUF_SIZE; //128k
    netlib_option(handle, NETLIB_OPT_SET_SEND_BUF_SIZE, &socket_buf_size);
    netlib_option(handle, NETLIB_OPT_SET_RECV_BUF_SIZE, &socket_buf_size);
}

void FileClientConn::OnClose() {
    log("client onclose:handle=%d", m_handle);
    Close();
}

void FileClientConn::OnTimer(uint64_t curr_tick) {
    if(transfer_task_ && transfer_task_->GetTransMode() == FILE_TYPE_ONLINE) {//在线传输
        if(transfer_task_->state() == kTransferTaskStateInvalid) {//文件无用的状态，则关闭连接
            log("Close another online conn, user_id=%d", user_id_);
            Close();
            return;
        }
    }
    if(curr_tick > m_last_recv_tick + CLIENT_TIMEOUT) { //两分钟
        log("client timeout, user_id=%u", user_id_);
        Close();
    }
}

void FileClientConn::OnWrite() {
    CImConn::OnWrite();
}

void FileClientConn::HandlePdu(CImPdu* pdu) {
    switch(pdu->GetCommandId()) {
        case CID_OTHER_HEARTBEAT:
            _HandleHeartBeat(pdu);
            break;
        case CID_FILE_LOGIN_REQ:
            _HandleClientFileLoginReq(pdu);
            break;
        case CID_FILE_STATE:
            _HandleClientFileStates(pdu);
            break;
        case CID_FILE_PULL_DATA_REQ:
            _HandleClientFilePullFileReq(pdu);
            break;
        case CID_FILE_PULL_DATA_RSP:
            _HandleClientFilePullFileRsp(pdu);
            break;
        default:
            log("no such cmd id:%u", pdu->GetCommandId());
            break;
    }
}

void FileClientConn::_HandleHeartBeat(CImPdu* pdu) {
    SendPdu(pdu);
}

void FileClientConn::_HandleClientFileLoginReq(CImPdu* pdu) {
    IM::File::IMFileLoginReq login_req;
    CHECK_PB_PARSE_MSG(login_req.ParseFromArray(pdu->GetBodyData(), pdu->GetBodyLength()));
    uint32_t user_id = login_req.user_id();
    string task_id = login_req.task_id();

    //共有四种角色，发送者，接收者， 上传者，下载者，前两者用于在线传输
    IM::BaseDefine::ClientFileRole mode = login_req.file_role();

    log("client login, user_id=%d, task_id=%s, file_role=%d", user_id, task_id.c_str(), mode);
    BaseTransferTask* transfer_task = nullptr;
    bool rv = false;
    do {
        //查找任务是否存在
        transfer_task = TransferTaskManager::GetInstance()->FindByTaskID(task_id);

        if(transfer_task == nullptr) {
            //看看是否是离线文件
            if(mode == CLIENT_OFFLINE_DOWNLOAD) { //如果是下载者
                //文件不存在，有可能是文件服务器重启，map的映射关系不在了

                //尝试从磁盘加载
                transfer_task = TransferTaskManager::GetInstance()->NewTransferTask(task_id, user_id);
                //需要再次判断是否加载成功
                if(transfer_task == nullptr) {
                    log("find task id failed, user_id=%u, task_id=%s, mode=%d",user_id, task_id.c_str(), mode);
                    break;
                }
            } else {
                log("Can't find task_id, user_id=%u, task_id=%s, mode=%d", user_id, task_id.c_str(), mode);
                break;
            }
        }
        //状态转换，这个接口，有可能是在线传输的情况，也可能是离线传输的情况
        rv = transfer_task->ChangePullState(user_id,mode);
        if(!rv) {
            break;
        }

        auth_ = true; //身份验证
        transfer_task_ = transfer_task;
        user_id_ = user_id;

        //设置conn，根据user_id 确定是发送方conn， 还是接收方conn,
        transfer_task->SetConnByUserID(user_id, this);
        rv = true;
    } while(0);
    IM::File::IMFileLoginRsp login_rsp;
    login_rsp.set_result_code(rv ? 0 : 1); //正常进行返回0， 失败返回1
    login_rsp.set_task_id(task_id);

    ::SendMessageLite(this, SID_FILE, CID_FILE_LOGIN_RES, pdu->GetSeqNum(), &login_rsp);
    if(rv) { //成功获取到task， 并且更换好状态
        if(transfer_task->GetTransMode() == FILE_TYPE_ONLINE) { //在线传输情况

        //  必须是发送方和接收方都准备好了才会进入这个语句
            if(transfer_task->state() == kTransferTaskStateWaitingTransfer) {
                CImConn* conn = transfer_task_->GetToConn();
                if(conn) {
                    //通知文件接收方
                    //对应到客户端就是，出现那个是否接收文件的弹框
                    _StatesNotify(CLIENT_FILE_PEER_READY, task_id, transfer_task_->from_user_id(), conn);
                } else {
                    log("to_conn is close, close me!");
                    Close();
                }
            }
        } else {  //离线传输情况

        //第一次进入ChangePullState(), 就会切换为以下这种状态 
        // 这个函数只会进入两次，一次上传者一次下载者
        //（这是上传者来时才会进入的判断）
            if(transfer_task->state() == kTransferTaskStateWaitingUpload) {
                OfflineTransferTask* offline = reinterpret_cast<OfflineTransferTask*>(transfer_task);
                IM::File::IMFilePullDataReq pull_data_req;
                pull_data_req.set_task_id(task_id);
                pull_data_req.set_user_id(user_id);
                pull_data_req.set_trans_mode(FILE_TYPE_OFFLINE);
                pull_data_req.set_offset(0);
                pull_data_req.set_data_size(offline->GetNextSegmentBlockSize());

                //向上传者请求数据，注意这是离线传输的情况
                ::SendMessageLite(this, SID_FILE, CID_FILE_PULL_DATA_REQ, &pull_data_req);
                log("pull data req");
            }
        }
    } else {
        Close();
    }
}

void FileClientConn::_HandleClientFileStates(CImPdu* pdu) {
    if(!auth_ || !transfer_task_) {//167行auth通过
        log("recv a client_file_state, but auth is false");
        return ;
    }
    IM::File::IMFileState file_state;
    CHECK_PB_PARSE_MSG(file_state.ParseFromArray(pdu->GetBodyData(), pdu->GetBodyLength()));
    string task_id = file_state.task_id();
    uint32_t user_id = file_state.user_id();
    uint32_t file_stat = file_state.state();
    log("recv filestate,user_id=%d, task_id=%s, file_stat=%d", user_id, task_id.c_str(),file_stat);
    bool rv = false;
    do {
        //检查user_id
        if (user_id != user_id_) {
            log("Received user_id valid, recv_user_id = %d, transfer_task.user_id = %d, user_id_ = %d", user_id, transfer_task_->from_user_id(), user_id_);
            break;
        }

        //检查task_id
        if (transfer_task_->task_id() != task_id) {
            log("Received task_id valid, recv_task_id = %s, this_task_id = %s", task_id.c_str(), transfer_task_->task_id().c_str());
            break;
        }
        
        switch(file_stat) {
            case CLIENT_FILE_CANCEL:
            case CLIENT_FILE_DONE:
            case CLIENT_FILE_REFUSE:
            {
                CImConn* im_conn = transfer_task_->GetOpponentConn(user_id);
                if(im_conn) {
                    im_conn->SendPdu(pdu);
                    log("Task %s %d by user_id:%d notify %d, erased", task_id.c_str(), file_stat, user_id, transfer_task_->GetOpponent(user_id));
                }
                rv = true;
                break;
            }
            default:
                log("Recv valid file_stat: file_state = %d, user_id=%d, task_id=%s", file_stat, user_id_, task_id.c_str());
                break;
        }
    } while(0);
    Close();
}

void FileClientConn::_HandleClientFilePullFileReq(CImPdu* pdu) {
    if(!auth_ || !transfer_task_) {
        log("recv a client_file_state, but auth is false");
        return;
    }
    IM::File::IMFilePullDataReq pull_data_req;
    CHECK_PB_PARSE_MSG(pull_data_req.ParseFromArray(pdu->GetBodyData(), pdu->GetBodyLength()));
    uint32_t user_id = pull_data_req.user_id();
    string task_id = pull_data_req.task_id();
    uint32_t mode = pull_data_req.trans_mode();
    uint32_t offset = pull_data_req.offset();
    uint32_t datasize = pull_data_req.data_size();
    log("Recv FilePullFileReq, user_id=%d, task_id=%s, file_role=%d, offset=%d, datasize=%d", user_id, task_id.c_str(), mode, offset, datasize);

    //RSP
    IM::File::IMFilePullDataRsp pull_data_rsp;
    pull_data_rsp.set_result_code(1); //返回1是有问题的，所以后边如果逻辑通过会修改为0
    pull_data_rsp.set_task_id(task_id);
    pull_data_rsp.set_user_id(user_id);
    pull_data_rsp.set_offset(offset);
    pull_data_rsp.set_file_data("");

    int rv = -1;
    do {
        //检查user_id;
        if(user_id != user_id_) {
            log("Received user_id valid, recv_user_id = %d, transfer_task.user_id = %d, user_id_ = %d", user_id, transfer_task_->from_user_id(), user_id_);
            break;
        }
        //检查task_id， 确认是否有这个任务
        if (transfer_task_->task_id() != task_id) { 
            log("Received task_id valid, recv_task_id = %s, this_task_id = %s", task_id.c_str(), transfer_task_->task_id().c_str());
            break;
        }

        //离线传输，需要下载文件
        //在线传输，从发送者拉数据
        //user_id 为transfer_task.to_user_id, 因为存文件的时候就是用的to_user_id;
        if (!transfer_task_->CheckToUserID(user_id)) { //看看是否是to_user_id;
            log("user_id equal transfer_task.to_user_id, but user_id=%d, transfer_task.to_user_id=%d", user_id, transfer_task_->to_user_id());
            break;
        }
                                                                                        //byte类型
        rv = transfer_task_->DoPullFileRequest(user_id, offset, datasize, pull_data_rsp.mutable_file_data());
        if(rv == -1) {
            break;
        }
        pull_data_rsp.set_result_code(0);
        if(transfer_task_->GetTransMode() == FILE_TYPE_ONLINE) {
            OnlineTransferTask* online = reinterpret_cast<OnlineTransferTask*>(transfer_task_);
            online->SetSeqNum(pdu->GetSeqNum());
            CImConn* conn = transfer_task_->GetOpponentConn(user_id);
            if(conn) {
                conn->SendPdu(pdu);
            }
        } else {
            SendMessageLite(this, SID_FILE, CID_FILE_PULL_DATA_RSP, pdu->GetSeqNum(), &pull_data_rsp);
            if(rv == 1) { // 1表示读完了
                _StatesNotify(CLIENT_FILE_DONE, task_id, transfer_task_->from_user_id(), this);
            }
        }
    } while(0);
    if(rv != 0) { //正在读的情况返回0 ， 错误返回- 1， 读完返回1
        Close();
    }
}

void FileClientConn::_HandleClientFilePullFileRsp(CImPdu* pdu) {
    if(!auth_ || !transfer_task_) {
        log("auth is false");
        return ;
    }
    //只有rsp
    IM::File::IMFilePullDataRsp pull_data_rsp;
    CHECK_PB_PARSE_MSG(pull_data_rsp.ParseFromArray(pdu->GetBodyData(), pdu->GetBodyLength()));
    uint32_t user_id = pull_data_rsp.user_id();
    string task_id = pull_data_rsp.task_id();
    uint32_t offset = pull_data_rsp.offset();
    uint32_t data_size = static_cast<uint32_t>(pull_data_rsp.file_data().length());
    const char* data = pull_data_rsp.file_data().data();
    log("Recv FilePullFileRsp, task_id=%s, user_id=%u, offset=%u, data_size=%d", task_id.c_str(), user_id, offset, data_size);
    
    int rv = 01;
    do {
        //检查user_id;
        if (user_id != user_id_) {
            log("Received user_id valid, recv_user_id = %d, transfer_task.user_id = %d, user_id_ = %d", user_id, transfer_task_->from_user_id(), user_id_);
            break;
        }
        
        // 检查task_id
        if (transfer_task_->task_id() != task_id) {
            log("Received task_id valid, recv_task_id = %s, this_task_id = %s", task_id.c_str(), transfer_task_->task_id().c_str());
            // Close();
            break;
        }
        //以下这个函数分在线和离线状态， 对于在线，则只是检查状态， 若离线则将数据缓存到本地
        rv = transfer_task_->DoRecvData(user_id, offset, data, data_size);
 
        if(rv == -1) break;
        if(transfer_task_->GetTransMode() == FILE_TYPE_ONLINE) {
            //对于在线，直接转发
            OnlineTransferTask* online = reinterpret_cast<OnlineTransferTask*>(transfer_task_);
            pdu->SetSeqNum(online->GetSeqNum());
            CImConn* conn = transfer_task_->GetToConn();
            if(conn) {
                conn->SendPdu(pdu);
            }
        } else { //离线
            if(rv == 1) { //文件接收完毕，返回CLIENT_FILE_DONE命令
                _StatesNotify(CLIENT_FILE_DONE, task_id, user_id, this);
            } else {//不断向上传者请求数据
                OfflineTransferTask* offline = reinterpret_cast<OfflineTransferTask*>(transfer_task_);
                IM::File::IMFilePullDataReq pull_data_req;
                pull_data_req.set_task_id(task_id);
                pull_data_req.set_user_id(user_id);
                pull_data_req.set_trans_mode(static_cast<IM::BaseDefine::TransferFileType>(offline->GetTransMode()));
                pull_data_req.set_offset(offline->GetNextSegmentBlockSize());
                ::SendMessageLite(this, SID_FILE, CID_FILE_PULL_DATA_REQ, &pull_data_req);
            }
        }
    } while(0);
    if (rv != 0) {
        //-1 失败
        // 1 离线文件上传完成
        Close();
    }
}

int FileClientConn::_StatesNotify(int state, const std::string& task_id, uint32_t user_id, CImConn* conn) {
    FileClientConn* file_client_conn = reinterpret_cast<FileClientConn*>(conn);
    IM::File::IMFileState file_msg;
    file_msg.set_state(static_cast<ClientFileState>(state));
    file_msg.set_task_id(task_id);
    file_msg.set_user_id(user_id);

    ::SendMessageLite(conn, SID_FILE, CID_FILE_STATE, &file_msg);
    log("notify to user %d state %d task %s", user_id, state, task_id.c_str());
    return 0;
}