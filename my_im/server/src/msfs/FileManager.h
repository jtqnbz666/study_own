#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_

#include <map>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include <errno.h>
#include <time.h>
#include "FileLin.h"
#include "CriticalSection.h"

using namespace std;

namespace msfs {
class CriticalSection;
class FileManager {
private:
    FileManager() {}
    FileManager(const char* host, const char* disk, 
                int totFiles, int filesPerDir) {
        m_host = new char[strlen(host) + 1];
        m_disk = new char[strlen(disk) + 1];
        m_host[strlen(host)] = '\0';
        m_disk[strlen(disk)] = '\0';
        strncpy(m_host, host, strlen(host));
        strncpy(m_disk, disk, strlen(disk));
        m_totFiles = totFiles;
        m_filesPerDir = filesPerDir;
        m_map.clear();                       
    }

    ~FileManager() {
        if(m_host) {
            delete []m_host;
        }
        m_host = nullptr;
        if(m_disk) delete []m_disk;
        m_disk = nullptr;
        EntryMap::iterator it = m_map.begin();
        while(it != m_map.end()) {
            delete it->second;
            m_map.erase(it ++);
        }
    }
    
    FileManager(const FileManager&);
    FileManager operator=(const FileManager&);

public:
    static FileManager* getInstance(const char* host, const char* disk, int totFiles, int filesPerDir) {
        return (m_instance) ? m_instance : \
                        (new FileManager(host, disk, totFiles, filesPerDir));
    }
    static void destroyInstance() {
        if(m_instance) delete m_instance;
        m_instance = nullptr;
    }

    //一开始就创建一级目录255个，每一个一级目录又有255个二级目录
    int initDir();
    u64 getFileCntCurr() { return m_totFiles; }
    int getFirstDir() { return (m_totFiles / (m_filesPerDir)) / (FIRST_DIR_MAX); }
    int getSecondDir() { return (m_totFiles % (m_filesPerDir * FIRST_DIR_MAX)) / m_filesPerDir; }

    string createFileRelatePath();
    int uploadFile(const char* type, const void* content, u32 size, char* url, char* ext = nullptr);
    int downloadFileByUrl(char* urlEn, void* buf, u32* size);

    //获取第一个"/" 之后的数据, 即获取相对路径
    int getRelatePathByUrl(const string& url, string& path);

    //获取绝对路径
    int getAbsPathByUrl(const string& url, string& path);

protected:
    //      条目，可以理解为对于一个文件的封装
    struct Entry {
        time_t m_lastAccess; //记录最新的访问时间
        size_t m_fileSize;
        u8* m_fileContent;
        Entry() {
            m_lastAccess = 0;
            m_fileSize = 0;
            m_fileContent = nullptr;
        }
        ~Entry() {
            if(m_fileContent) {
                delete []m_fileContent;
            }
            m_fileContent = nullptr;
        }
    };
    typedef std::map<std::string, Entry*> EntryMap;
    int insertEntry(const std::string& url, size_t filesize, const void* content);
    Entry* getEntry(const std::string& url) const {
        return const_cast<FileManager*>(this)->getOrCreateEntry(url, false);
    }

    //获取或者创建一个条目
    Entry* getOrCreateEntry(const std::string& url, bool create);
    void releaseFileCache(const std::string& url);

    //更新m_map中的映射关系， 超过两小时未访问的映射就剔除
    void updateMapCache();

private:
    char*               m_host; //msfs ip 或者 hostname
    char*               m_disk; //存储资源的目录路径, 这里名字为tmp
    u64                 m_totFiles; //总共存储的数量

    CriticalSection     m_filesCs;//锁

    int                 m_filesPerDir;
    static const u32    MAX_FILE_SIZE_PER_FILE = 5 * 1024 * 1024;
    static const int    FIRST_DIR_MAX = 255;
    static const int    SECOND_DIR_MAX = 255;
    static FileManager* m_instance;
    EntryMap m_map;
    static const u64    MAX_FILE_IN_MAP = 10000;
    CriticalSection     m_cs; //锁
};
}

#endif