#ifndef FILE_SERVER_FILE_CLIENT_CONN_H_
#define FILE_SERVER_FILE_CLIENT_CONN_H_

#include "base/imconn.h"
#include "transfer_task.h"

//异常情况处理
//断线、服务器重启等
class FileClientConn : public CImConn {
public:
    FileClientConn() : auth_(false), user_id_(0), transfer_task_(nullptr) {

    }
    virtual ~FileClientConn() {}
    virtual void Close();
    
    virtual void OnConnect(net_handle_t handle);
    virtual void OnClose();
    virtual void OnTimer(uint64_t curr_tick);
    
    virtual void OnWrite();
    virtual void HandlePdu(CImPdu* pdu);

    void ClearTransferTask() {
        user_id_ = 0;
        transfer_task_ = nullptr;
    }

private:
    void _HandleHeartBeat(CImPdu* pdu);

    //不管在线离线都会进入下边这个函数
    void _HandleClientFileLoginReq(CImPdu* pdu);
    
    void _HandleClientFileStates(CImPdu* pdu);
    void _HandleClientFilePullFileReq(CImPdu* pdu);
    void _HandleClientFilePullFileRsp(CImPdu* pdu);

    int _StatesNotify(int state, const std::string& task_id, uint32_t user_id, CImConn* conn);


private:
    int _PreUpload(const char* task_id);
    bool                auth_;
    uint32_t            user_id_;
    //每个连接对应一次任务，故可预先缓存
    BaseTransferTask* transfer_task_;
};

void InitializeFileClientConn();
void FileClientConnCallback(void* callback_data, uint8_t msg, uint32_t handle, void* param);

#endif