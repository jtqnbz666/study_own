#ifndef __HTTP_CURL_H__
#define __HTTP_CURL_H__
#include <string>
#include <curl/curl.h> // 配合curl使用
#include "public_define.h"

class CHttpClient {
public: 
    CHttpClient(void);
    ~CHttpClient(void);

public: 
    CURLcode Post(const string& strUrl, const string& strPost, string& strResponse);
    CURLcode Get(const string& strUrl, string& strResponse);
    string UploadByteFile(const string& url, void* data, int data_len); //上传资源,如头像
    bool DownloadByteFile(const string& url, AudioMsgInfo* pAudioMsg); //下载资源
};

#endif