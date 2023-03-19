#ifndef WIN32
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>

#include "FileLin.h"
namespace msfs {
File::File(const char* path) {
    assert(path);
    m_path = new char[strlen(path) + 1];
    strncpy(m_path, path, strlen(path));
    m_path[strlen(path)] = '\0';
    m_opened = false;
    m_file = -1;
    m_size = -1;
}

File::~File() {
    if(m_opened) {
        close();
    }
    delete []m_path;
    m_path = NULL;
}

u64 File::isExist(bool* exist) {
    //检查调用进程是否可以对指定的文件执行某种操作。
    //成功执行时，返回0。失败返回-1
    *exist = access(m_path, 0) == 0;
    return 0;
}

u64 File::remove() {
    if(-1 == unlink(m_path)) {
        return errno;
    }
    return 0;
}

u64 File::create(bool directIo) {
    assert(!m_opened);
    int flags = O_RDWR | O_CREAT | O_EXCL;
 
	m_file = open64(m_path, flags, 00640);
    if(-1 == m_file) return errno;

    m_opened = true;
    m_size = 0;
    m_directIo = directIo;
    return 0;
}

u64 File::open(bool directIo) {
    assert(!m_opened);
    int flags = O_RDWR;

	m_file = open64(m_path, flags);
	if(-1 == m_file) {
		return errno;
	}

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;
    if(fcntl(m_file, F_SETLK, &lock) < 0) {
        ::close(m_file);
        return errno;
    }

    m_opened = true;
    u64 size = 0;
    u64 code = getSize(&size);
    if(code) {
        close();
        return code;
    }
    m_size = size;
    m_directIo = directIo;
    return 0;
}

u64 File::close() {
    if(!m_opened) return 0;
    //调用sync可以将系统缓冲区（内存中）的数据写入到文件系统（磁盘）中。
    //成功返回0， ，失败返回-1；
    u64 err = sync();
    if(err) return err;
    m_opened = false;
    if(0 != ::close(m_file)) return errno;
    return 0;
}

s64 File::getFileSize(const char* path) {
    s64 filesize = -1;
    struct stat statbuff;
    if(stat(path, &statbuff) < 0) {
        return filesize;
    } else {
        filesize = statbuff.st_size;
    }
    return filesize;
}

u64 File::getSize(u64* size) {
    if(m_size > 0) {
        *size = m_size;
        return 0;
    }
    s64 sksize = lseek(m_file, 0, SEEK_END);
    if(-1 == sksize) return errno;
    else *size = (u64)sksize;
    m_size = *size;
    return 0;
}

u64 File::setSize(u64 size) {
    assert(m_opened);
    //将参数fd指定的文件大小改为参数length指定的大小。参数fd为已打开的文件描述词，
    //而且必须是以写入模式打开的文件。如果原来的文件件大小比参数length大，则超过的部分会被删去
	if (-1 == ftruncate64(m_file, size))

    return errno;
    m_size = size;
    return 0;
}

u64 File::read(u64 offset, u32 size, void* buffer) {
    assert(m_opened);
    if(offset + size > (u64)m_size) return E_EOF;
    if(size != pread64(m_file, buffer, size, offset)) {
        return errno;
    }
    return 0;
}

u64 File::write(u64 offset, u32 size, const void* buffer) {
    assert(m_opened);
    setSize((u64)size);
    if(offset + size > (u64)m_size) return E_EOF;
    if(size != pwrite64(m_file, buffer, size, offset)) {
        return errno;
    }
    return 0;
}

u64 File::sync() {
    assert(m_opened);
    if(-1 == fsync(m_file)) {
        return errno;
    }
    return 0;
}

u64 File::mkdirNoRecursion(const char* path) {
    if(::mkdir(path, 0777) != 0) return errno;
    return 0;
}

u64 File::isDirectory(bool* isDir) {
    struct stat fileStat;
    if(stat(m_path, &fileStat) != 0) return errno;
    *isDir = S_ISDIR(fileStat.st_mode) != 0;
    return 0;
}

u64 File::getFileNum(int* fileNum) {
    DIR* dp;
    struct dirent* ep = nullptr;
    string dir(m_path);
    if(dir[dir.length() - 1] != '/' && dir[dir.length() - 1] != '\\') {
        dir += '/';
    }
    dp = opendir(m_path);
    if(!dp) return errno;
    int files = 0;
    ep = readdir(dp);
    while(ep) {
        string subPath = dir + ep->d_name;
        File subFile(subPath.c_str());
        bool isDir = false;
        if(subFile.isDirectory(&isDir) != 0 || isDir) {
            closedir(dp);
            return errno;
        } else {
            if(strncmp(ep->d_name, ".", 1) != 0) {
                files ++;
            }
        }
       ep = readdir(dp);
    }
     *fileNum = files;
    closedir(dp);
    return 0;
}

}
#endif