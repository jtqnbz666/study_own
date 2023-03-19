#include "transfer_task.h"
#include <uuid/uuid.h>
#include "base/util.h"
#include "base/pb/protocol/IM.BaseDefine.pb.h"

using namespace IM::BaseDefine;

std::string GenerateUUID() {
    std::string rv;
    uuid_t uid = {0};
    uuid_generate(uid); //生成
    if(!uuid_is_null(uid)) {
        char str_uuid[64] = {0};
        uuid_unparse(uid, str_uuid);
        rv = str_uuid;
    }
    return rv;
}

const char* GetCurrentOfflinePath() {
    static const char* g_current_save_path = NULL;
    if(g_current_save_path == NULL) {
        static char s_tmp[BUFSIZ];
        char work_path[BUFSIZ];
        if(!getcwd(work_path, BUFSIZ)) {
            log("getcwd %s failed", work_path);
        } else {
            snprintf(s_tmp, BUFSIZ, "%s/offline_file", work_path);
        }

        log("save offline files to %s", s_tmp);
        int ret = mkdir(s_tmp, 0755);
        if( (ret != 0) && (errno != EEXIST) ) {
            log("!!!mkdir %s failed to save offline files", s_tmp);
        }
        
        g_current_save_path = s_tmp;
    }
    return g_current_save_path;
}

static FILE* OpenByRead(const std::string& task_id, uint32_t user_id) {
    FILE* fp = NULL;
    if(task_id.length() >= 2) {
        char save_path[BUFSIZ];

        //GetCurrentOfflinePath()得到一个路径
        snprintf(save_path, BUFSIZ, "%s/%s/%s", GetCurrentOfflinePath(), task_id.substr(0, 2).c_str(), task_id.c_str());
        fp = fopen(save_path, "rb"); //打开文件
        if(!fp) {
            log("Open file %s for read failed", save_path);
        }
    }
    return fp;
}

static FILE* OpenByWrite(const std::string& task_id, uint32_t user_id) {
    FILE* fp = NULL;
    if(task_id.length() >= 2) {
        char save_path[BUFSIZ];
        snprintf(save_path, BUFSIZ, "%s/%s", GetCurrentOfflinePath(), task_id.substr(0, 2).c_str());
        int ret = mkdir(save_path, 0755);
        if( (ret != 0) && (errno != EEXIST) ) {
            log("mkdir failed for path: %s", save_path);
        } else {
            strncat(save_path, "/", BUFSIZ);
            strncat(save_path, task_id.c_str(), BUFSIZ);

            fp = fopen(save_path, "ab+");
            if(!fp) {
                log("Open file for write failed");
            }
        }
    }
    return fp;
}

BaseTransferTask::BaseTransferTask(const std::string& task_id, uint32_t from_user_id, uint32_t to_user_id, const std::string& file_name, uint32_t file_size) :
task_id_(task_id), from_user_id_(from_user_id), to_user_id_(to_user_id), file_name_(file_name), file_size_(file_size), state_(kTransferTaskStateReady) {
    create_time_ = time(NULL);
    from_conn_ = NULL;
    to_conn_ = NULL;
}

void BaseTransferTask::SetLastUpdateTime() {
    create_time_ = time(NULL);
}

uint32_t OnlineTransferTask::GetTransMode() const {
    return IM::BaseDefine::FILE_TYPE_ONLINE;
}

bool OnlineTransferTask::ChangePullState(uint32_t user_id, int file_role) {
    //在线文件传输， 初始状态： kTransferTaskStateReady
    //状态转换流程， kTransferTaskStateReady
    //-->kTransferTaskStateWaitingSender或者kTransferTaskStateWaitingReceiver
    //-->kTransferTaskStateWaitingTransfer

    bool rv = false;
    do {
        //检验user_id 和 file_role是否对应的上
        rv = CheckByUserIDAndFileRole(user_id, file_role);
        if(!rv) {
            log("Check error!  user_id = %d, file_role = %d", user_id, file_role);
            break;
        }

        if(state_ != kTransferTaskStateReady && state_ != kTransferTaskStateWaitingSender && state_ != kTransferTaskStateWaitingReceiver) {
            log("Invalid state, valid state is kTransferTaskStateReady or kTransferTaskStateWaitingSender or kTransferTaskStateWaitingReceiver, but state is %d", state_);
            break;
        }

        if(state_ == kTransferTaskStateReady) {
            //第一个用户进来(并且是第一次)，
            //如果是sender， 则-->kTransferTaskStateWaitingReceiver;
            //如果是receiver， 则-->kTransferTaskStateWaitingSender;
            if(file_role == CLIENT_REALTIME_SENDER) {
                state_ = kTransferTaskStateWaitingReceiver;
            } else {
                state_ = kTransferTaskStateWaitingSender;
            }
        } else {
            if (state_ == kTransferTaskStateWaitingReceiver) { //需要receiver
                //需要检查来的是否是receiver
                if(file_role != CLIENT_REALTIME_RECVER) {
                    log("Invalid user, user_id = %d, but to_user_id_ = %d", user_id, to_user_id_);
                    break;
                }
            } else if (state_ == kTransferTaskStateWaitingSender) { //需要sender
                if(file_role != CLIENT_REALTIME_SENDER) {
                    log("Invalid user, user_id = %d, but to_user_id_ = %d", user_id, to_user_id_);
                    break;
                }
            }
            
            state_ = kTransferTaskStateWaitingTransfer;//发送接收都准备好了
        }
        SetLastUpdateTime();
        rv = true;
    } while(0);
    return rv;
}

bool OnlineTransferTask::CheckByUserIDAndFileRole(uint32_t user_id, int file_role) const {
    //在线文件传输
    //1.file_role必须是sender或receiver
    //sender的user_id 必须是 from_user_id_, 反之是 to_user_id_;
    bool rv = false;
    if(file_role == CLIENT_REALTIME_SENDER) {
        if(CheckFromUserID(user_id)) {
            rv = true;
        }
    } else if (file_role == CLIENT_REALTIME_RECVER) {
        if(CheckToUserID(user_id)) {
            rv = true;
        }
    }
    return rv;
}

int OnlineTransferTask::DoRecvData(uint32_t user_id, uint32_t offset, const char* data, uint32_t data_size) {
    int rv = -1;
    do {
        //检查是否是发送者
        if(!CheckFromUserID(user_id)) {
            log("Check error! user_id=%d, from_user_id=%d, to_user_id", user_id, from_user_id_, to_user_id_);
            break;
        }

        //检查状态
        if(state_ != kTransferTaskStateWaitingTransfer && state_ != kTransferTaskStateTransfering) {
            log("Check state_! user_id=%d, state=%d, but state need kTransferTaskStateWaitingTransfer or kTransferTaskStateTransfering", user_id, state_);
            break;
        }

        //更改状态
        if(state_ == kTransferTaskStateWaitingTransfer) {
            state_ = kTransferTaskStateTransfering;
        }
        SetLastUpdateTime();
        rv = 0;
    }while(0);
    return rv;
}

int OnlineTransferTask::DoPullFileRequest(uint32_t user_id, uint32_t offset, uint32_t data_size, std::string* data) {
    int rv = -1;
    //在线
    do {
        //检查状态
        if (state_ != kTransferTaskStateWaitingTransfer && state_ != kTransferTaskStateTransfering) {
            log("Check state_! user_id=%d, state=%d, but state need kTransferTaskStateWaitingTransfer or kTransferTaskStateTransfering", user_id, state_);
            break;
        }
        
        if (state_ == kTransferTaskStateWaitingTransfer) {
            state_ = kTransferTaskStateTransfering;
        }
        
        SetLastUpdateTime();
        rv = 0;
    }while(0);
    return rv;
}

OfflineTransferTask* OfflineTransferTask::LoadFromDisk(const std::string& task_id, uint32_t user_id) {
    OfflineTransferTask* offline = NULL;
    FILE* fp = OpenByRead(task_id, user_id); //存储文件的时候只用到了task_id,没用user_id;
    if(fp) { 
        OfflineFileHeader file_header;
        //先把头读出来， 说明存的时候也是先存了头
        size_t size = fread(&file_header, 1, sizeof(file_header), fp);
        if(size == sizeof(file_header)) {
            fseek(fp, 0L, SEEK_END);
            //得到文件内容大小
            size_t file_size = static_cast<size_t>(ftell(fp)) - size;
            //比对是否正确
            if(file_size == file_header.get_file_size()) {
                offline = new OfflineTransferTask(file_header.get_task_id(),
                                                  file_header.get_from_user_id(),
                                                  file_header.get_to_user_id(),
                                                  file_header.get_file_name(),
                                                  file_header.get_file_size());
                if(offline) {
                    //设置为等待下载状态
                    offline->set_state(kTransferTaskStateWaitingDownload);
                }
            } else {
                 log("Offile file size by task_id=%s, user_id=%u, header_file_size=%u, disk_file_size=%u", task_id.c_str(), user_id, file_header.get_file_size(), file_size);
            }
        } else {
             log("Read file_header error by task_id=%s, user_id=%u", task_id.c_str(), user_id);
        }
        fclose(fp);
    }
    return offline;
}

uint32_t OfflineTransferTask::GetTransMode() const {
    return IM::BaseDefine::FILE_TYPE_OFFLINE;
}

//离线传输的这个函数和在线传输不一样
//按道理只会进入这个函数两次， 一次是上传者进入，一次是下载者进入
bool OfflineTransferTask::ChangePullState(uint32_t user_id, int file_role) {
    ///离线文件传输
    //如果是发送者，状态转换kTransferTaskStateReady --> kTransferTaskStateWaitingUpload;
    //如果是接收者，状态转换kTransferTaskStateUploadEnd --> kTransferTaskStateWaitingDownload;
    bool rv = false;
    do {
        //检查user_id 和 角色 是否对应的上， 成功返回 1， 失败返回0 
        rv = CheckByUserIDAndFileRole(user_id, file_role);
        if(!rv) {
            log("Check error! user_id=%d, file_role=%d", user_id, file_role);
            break;
        }

        //只能是以下三种状态之一。
        if(state_ != kTransferTaskStateReady &&     //万事具备
        state_ != kTransferTaskStateUploadEnd &&    //上传完毕
        state_ != kTransferTaskStateWaitingDownload) { //等待下载状态
            log("Invalid state, valid state is kTransferTaskStateReady or kTransferTaskStateUploadEnd, but state is %d", state_);
            break;
        }
        if(state_ == kTransferTaskStateReady) { //对于离线传输，第一次进入这个函数的情况
            //第一个用户进来，必须是CLIENT_OFFLINE_UPLOAD(上传者)
            //必须是kTransferTaskStateReady,则--> kTransferTaskStateWaitingUpload
            if(CLIENT_OFFLINE_UPLOAD == file_role) { //角色有四个，还有一个DOWNLOAD,前两个是在线文件的，后两个是离线文件的
                state_ = kTransferTaskStateWaitingUpload; //切换为等待上传状态
            } else {
                log("Offline upload: file_role is CLIENT_OFFLINE_UPLOAD but file_role = %d", file_role);
                break;
            }
        } else {    // 不是第一次进入这个函数
            if(file_role == CLIENT_OFFLINE_DOWNLOAD) {  //下载者
                state_ = kTransferTaskStateWaitingDownload; //切换为等待下载
            } else {
                log("Offline upload: file_role is CLIENT_OFFLINE_DOWNLOAD but file_role = %d", file_role);
                break;
            }
        }
        SetLastUpdateTime(); //更新create_time为当前时间
        rv = true;
    } while(0);
    return rv;
}

bool OfflineTransferTask::CheckByUserIDAndFileRole(uint32_t user_id, int file_role) const {
    //离线文件传输
    //1. file_role必须是CLIENT_OFFLINE_UPLOAD或CLIENT_OFFLINE_DOWNLOAD
    //2. UPLOAD 则 user_id = from_user_id_
    //3. DOWNLOAD 则 user_id = to_user_id_
    bool rv = false;
    if(file_role == CLIENT_OFFLINE_UPLOAD) {
        if(CheckFromUserID(user_id)) {
            rv = true;
        }
    } else if (file_role == CLIENT_OFFLINE_DOWNLOAD) {
        if(CheckToUserID(user_id)) {
            rv = true;
        }
    }
    return rv;
}

int OfflineTransferTask::DoRecvData(uint32_t user_id, uint32_t offset, const char* data, uint32_t data_size) {
    //离线文件上传
    int rv = -1;
    do {
        //检查是否是发送者
        if(!CheckFromUserID(user_id)) {
            log("rsp user_id=%d, but sender_id is %d", user_id, from_user_id_);
            break;
        }

        //检查状态
        if(state_ != kTransferTaskStateWaitingUpload && state_ != kTransferTaskStateUploading) {
            log("state=%d error, need kTransferTaskStateWaitingUpload or kTransferTaskStateUploading", state_);
            break;
        }

        //检查offset是否有效
        if(offset != transfered_idx_ * SEGMENT_SIZE) {
            break;
        }

        data_size = GetNextSegmentBlockSize();
        log("Ready recv data, offset=%d, data_size=%d, segment_size=%d", offset, data_size, segment_size_);

        if(state_ == kTransferTaskStateWaitingUpload) {
            if(fp_ == nullptr) {
                fp_ = OpenByWrite(task_id_, to_user_id_);
                if(fp_ == nullptr) break;
            }

            //写文件头部 , 第一次进来才需要写
            OfflineFileHeader file_header;
            memset(&file_header, 0, sizeof(file_header));
            file_header.set_create_time(time(NULL));
            file_header.set_task_id(task_id_);
            file_header.set_from_user_id(from_user_id_);
            file_header.set_to_user_id(to_user_id_);
            file_header.set_file_name("");
            file_header.set_file_size(file_size_);
            fwrite(&file_header, 1, sizeof(file_header), fp_);
            fflush(fp_); // 将缓冲区的内容写到fp_中

            state_ = kTransferTaskStateUploading;
        }
        
        //存储
        if(fp_ == nullptr) {
            break;
        }
        fwrite(data, 1, data_size, fp_);
        fflush(fp_);
        ++transfered_idx_;
        SetLastUpdateTime();
        if(transfered_idx_ == segment_size_) {
            state_ = kTransferTaskStateUploadEnd;
            fclose(fp_);
            fp_ = nullptr;
            rv = 1;
        } else {
            rv = 0;
        }
    } while(0);
    return rv;
}

int OfflineTransferTask::DoPullFileRequest(uint32_t user_id, uint32_t offset, uint32_t data_size, std::string* data) {
    int rv = -1;
    log("Recv pull file request:user_id=%d, offset=%d, data_size=%d",user_id, offset, data_size);
    do {
        //首先检查状态，必须是必须为kTransferTaskStateWaitingDownload或kTransferTaskStateDownloading
        if (state_ != kTransferTaskStateWaitingDownload && state_ != kTransferTaskStateDownloading) {
            log("state=%d error, need kTransferTaskStateWaitingDownload or kTransferTaskStateDownloading", state_);
            break;
        }

        //处理kTransferTaskStateWaitingDownload
        if(state_ == kTransferTaskStateWaitingDownload) {
            if(transfered_idx_ != 0) {
                transfered_idx_ = 0;
            }
            if(fp_ != nullptr) {
                fclose(fp_);
                fp_ = nullptr;
            }
            fp_ = OpenByRead(task_id_, user_id); //因为写文件的时候，用的是to_user_id, 这里是下载者的id刚好对应
            if(fp_ == nullptr) break;

            OfflineFileHeader file_header;
            size_t size = fread(&file_header, 1, sizeof(file_header), fp_);
            if(sizeof(file_header) != size) {
                log("read file head failed.");
                fclose(fp_);
                fp_ = nullptr;
                break;
            }
            state_ = kTransferTaskStateDownloading;
        } else {
            //检查文件是否打开
            if(fp_ == nullptr) break;
        }

        //检查offset是否有效
        if(offset != transfered_idx_ * SEGMENT_SIZE) {
            log("Recv offset error, offser=%d, transfered_offset=%d", offset, transfered_idx_*SEGMENT_SIZE);
            break;
        }

        data_size = GetNextSegmentBlockSize();
        log("Ready send data, offset=%d, data_size=%d", offset, data_size);

        char* tmpbuf = new char[data_size];
        if(nullptr == tmpbuf) {
            log("alloc mem failed!");
            break;
        }
        memset(tmpbuf, 0, data_size);
        size_t size = fread(tmpbuf, 1, data_size, fp_);
        if(size != data_size) {
            log("read size error, data_size=%d, buf read_size=%d", data_size, size);
            delete []tmpbuf;
            break;
        }
        data->append(tmpbuf, data_size);
        delete []tmpbuf;
        transfered_idx_ ++;
        SetLastUpdateTime();
        if(transfered_idx_ == segment_size_) {
            log("Pull req end.");
            state_ = kTransferTaskStateUploadEnd;
            fclose(fp_);
            fp_ = nullptr;
            rv = 1;
        } else {
            rv = 0;
        }
    } while(0);
    return rv;
}