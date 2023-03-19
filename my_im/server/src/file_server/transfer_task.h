#ifndef FILE_SERVER_TRANSFER_TASK_H_
#define FILE_SERVER_TRANSFER_TASK_H_

#include "util.h"
#include "offline_file_util.h"

class CImConn; //类的前置声明，不然无法使用它定义对象，和函数一样需要前置声明


//状态机
enum TransferTaskState {
    kTransferTaskStateInvalid           = 0,    //非法
    kTransferTaskStateReady             = 1,    //准备好了 , 构造函数初始化完毕

    kTransferTaskStateWaitingSender     = 2,    //等待发送者
    kTransferTaskStateWaitingReceiver   = 3,    //等待接收者
    kTransferTaskStateWaitingTransfer   = 4,    //发送接收都准备好了 
    kTransferTaskStateTransfering       = 5,    //正在传输中
    kTransferTaskStateTransferDone      = 6,    //传输完毕
//上边是在线传输， 下边是离线传输状态

    kTransferTaskStateWaitingUpload     = 7,    //等待上传
    kTransferTaskStateUploading         = 8,    //正在上传中
    kTransferTaskStateUploadEnd         = 9,    //上传完毕
    
    kTransferTaskStateWaitingDownload   = 10,   //等待下载
    kTransferTaskStateDownloading       = 11,   //正在下载中
    kTransferTaskStateDownloadEnd       = 12,   //下载完毕

    kTransferTaskStateError             = 13,   //传输失败
};


//头结构和那几个头文件的都类似，offline_file_util.h file_server_util.h
struct OfflineFileHeader {
    OfflineFileHeader() {
        task_id[0] = '\0';
        from_user_id[0] = '\0';
        to_user_id[0] = '\0';
        create_time[0] = '\0';
        file_name[0] = '\0';
        file_size[0] = '\0';
    }

    void set_task_id(std::string& _task_id) {
        strncpy(task_id, _task_id.c_str(), 128 < _task_id.length() ? 128 : _task_id.length());
    }
    
    void set_from_user_id(uint32_t id) {
        sprintf(from_user_id, "%u", id);
    }
    
    void set_to_user_id(uint32_t id) {
        sprintf(to_user_id, "%u", id);
    }
    
    void set_create_time(time_t t) {
        sprintf(create_time, "%ld", t);
    }
    
    void set_file_name(const char* p) {
        sprintf(file_name, p, 512 < strlen(p) ? 512 : strlen(p));
    }
    
    void set_file_size(uint32_t size) {
        sprintf(file_size, "%u", size);
    }
    
    std::string get_task_id() const {
        return task_id;
    }
    
    uint32_t get_from_user_id() const {
        return string2int(std::string(from_user_id));
    }

    uint32_t get_to_user_id() const {
        return string2int(std::string(to_user_id));
    }

    uint32_t get_create_time() const {
        return string2int(std::string(create_time));
    }
    
    std::string get_file_name() const {
        return file_name;
    }

    uint32_t get_file_size() const {
        return string2int(std::string(file_size));
    }

    char task_id[128];
    char from_user_id[64];
    char to_user_id[64];
    char create_time[128];
    char file_name[512];
    char file_size[64];
};

//不管离线还是在线传输，都是它衍生出来的
class BaseTransferTask {
public:
    BaseTransferTask(const std::string& task_id, uint32_t from_user_id, uint32_t to_user_id, const std::string& file_name, uint32_t file_size);
    virtual ~BaseTransferTask() {}
    virtual uint32_t GetTransMode() const = 0; //纯虚函数
    inline const std::string& task_id() const { return task_id_; }
    inline uint32_t from_user_id() const { return from_user_id_; }
    inline uint32_t to_user_id() const { return to_user_id_; }
    inline uint32_t file_size() const { return file_size_; }
    inline const std::string& file_name() const { return file_name_; }
    inline time_t create_time() const { return create_time_; }
    inline void set_state(int state) { state_ = state; }
    inline int state() const { return state_; }

    //以下这几个函数在file_server_util.h也有
    uint32_t GetOpponent(uint32_t user_id) const {
        return (user_id == from_user_id_ ? user_id : from_user_id_);
    }

    CImConn* GetOpponentConn(uint32_t user_id) const {
        return (user_id == from_user_id_ ? to_conn_ : from_conn_);
    }

    CImConn* GetFromConn() {
        return from_conn_;
    }

    CImConn* GetToConn() {
        return to_conn_;
    }

    CImConn* GetConnByUserID(uint32_t user_id) const {
        if(from_user_id_ == user_id) {
            return from_conn_;
        } else if (to_user_id_ == user_id) {
            return to_conn_;
        } else {
            return nullptr;
        }
    }

    void SetConnByUserID(uint32_t user_id, CImConn* conn) {
        if(from_user_id_ == user_id) {
            from_conn_ = conn;
        } else if (to_user_id_ == user_id) {
            to_conn_ = conn;
        }
    }
    
    inline bool CheckFromUserID(uint32_t user_id) const {
        return from_user_id_ == user_id;
    }
    
    inline bool CheckToUserID(uint32_t user_id) const {
        return to_user_id_ == user_id;
    }

    inline bool CheckUserID(uint32_t user_id) const {
        return user_id == from_user_id_ || user_id == to_user_id_;
    }

    bool IsWaitTranfering() const {
        bool rv = false;
        if(state_ == kTransferTaskStateWaitingTransfer || state_ == kTransferTaskStateWaitingUpload || state_ == kTransferTaskStateWaitingDownload) {
            rv = true;
        }
        return rv;
    }

    void SetLastUpdateTime();

    //检查状态
    virtual bool ChangePullState(uint32_t user_id, int file_role) {
        return false; 
    }

    //检查输入是否合法
    virtual bool CheckByUserIDAndFileRole(uint32_t user_id, int file_role) const { return false; }
    virtual int DoRecvData(uint32_t user_id, uint32_t offset, const char* data, uint32_t data_size) { return -1; }
    virtual int DoPullFileRequest(uint32_t user_id, uint32_t offset, uint32_t data_size, std::string* data) { return -1; }

protected:
    std::string     task_id_;
    uint32_t        from_user_id_;
    uint32_t        to_user_id_;
    std::string     file_name_;
    uint32_t        file_size_;
    time_t          create_time_;
    int             state_;     //传输状态
    CImConn*        from_conn_;
    CImConn*        to_conn_;
};

typedef map<std::string, BaseTransferTask*> TransferTaskMap;// taskid(string类型) 和 任务 的映射
typedef map<CImConn*, BaseTransferTask*> TransferTaskConnkMap;

class OnlineTransferTask : public BaseTransferTask {
public:
    OnlineTransferTask(const std::string& task_id, uint32_t from_user_id, uint32_t to_user_id, const std::string& file_name, uint32_t file_size) :
    BaseTransferTask(task_id, from_user_id, to_user_id, file_name, file_size) {
        mac_seq_num_ = 0;//尽管是mac，先保留着
    }

    virtual ~OnlineTransferTask() {}
    virtual uint32_t GetTransMode() const;
    virtual bool ChangePullState(uint32_t user_id, int file_role);
    virtual bool CheckByUserIDAndFileRole(uint32_t user_id, int file_role) const;

    virtual int DoRecvData(uint32_t user_id, uint32_t offset, const char* data, uint32_t data_size);
    virtual int DoPullFileRequest(uint32_t user_id, uint32_t offset, uint32_t data_size, std::string* data);

    void SetSeqNum(uint32_t seq_num) {
        mac_seq_num_ = seq_num;
    }

    uint32_t GetSeqNum() const {
        return mac_seq_num_;
    }

private:
    uint32_t mac_seq_num_;
};

#define SEGMENT_SIZE 32768

class OfflineTransferTask : public BaseTransferTask {
public:
    OfflineTransferTask(const std::string& task_id, uint32_t from_user_id, uint32_t to_user_id, const std::string& file_name, uint32_t file_size) :
    BaseTransferTask(task_id, from_user_id, to_user_id, file_name, file_size) {
       // file_header_ = NULL;
       fp_ = nullptr;
       transfered_idx_ = 0;
       segment_size_ = SetMaxSegmentSize(file_size);
    }

    virtual ~OfflineTransferTask() {
        if(fp_) {
            fclose(fp_);
            fp_ = nullptr;
        }
    }

    static OfflineTransferTask* LoadFromDisk(const std::string& task_id, uint32_t user_id);
    virtual uint32_t GetTransMode() const;
    virtual bool ChangePullState(uint32_t user_id, int file_role);
    virtual bool CheckByUserIDAndFileRole(uint32_t user_id, int file_role) const;
    
    virtual int DoRecvData(uint32_t user_id, uint32_t offset, const char* data, uint32_t data_size);
    virtual int DoPullFileRequest(uint32_t user_id, uint32_t offset, uint32_t data_size, std::string* data);

    inline int GetSegmentSize() const {
        return segment_size_;
    }

    inline int GetNextSegmentBlockSize() {
        int block_size = SEGMENT_SIZE;
        if(transfered_idx_ + 1 == segment_size_) {
            block_size = file_size_ - transfered_idx_ * SEGMENT_SIZE;
        }
        return block_size;
    }

    uint32_t GetNextOffset() const {
        return SEGMENT_SIZE * transfered_idx_;
    }

private:
    //迭代器
    inline int SetMaxSegmentSize(uint32_t file_size) {
        int seg_size = file_size / SEGMENT_SIZE;
        if(file_size_ %SEGMENT_SIZE != 0) {
            seg_size = file_size / SEGMENT_SIZE + 1;
        }
        return seg_size;
    }

    FILE* fp_;
    //已经传输的
    int transfered_idx_;
    int segment_size_;
};

std::string GenerateUUID();
const char* GetCurrentOfflinePath();

#endif