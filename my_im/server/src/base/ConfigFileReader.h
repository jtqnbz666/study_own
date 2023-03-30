#ifndef CONFIGFILEREADER_H_
#define CONFIGFILEREADER_H_
//key-value的方式存储配置文件内容

#include "util.h"
class CConfigFileReader {
public:
    CConfigFileReader();
    CConfigFileReader(const char* filename);
    ~CConfigFileReader();
    CConfigFileReader& operator = (CConfigFileReader& A) {
        this->m_load_ok = A.m_load_ok;
        this->m_config_map = A.m_config_map;
        this->m_config_file = A.m_config_file;
        return *this;
    }
    // 获取配置配置文件中的某一个key对应的value
    char* GetConfigName(const char* filename);

    //修改或者增加配置文件的内容
    int SetConfigValue(const char* name, const char* value);
private:
    void _LoadFile(const char* filename);

    //把读出来的配置可以写到一个新的文件中去
    int _WriteFile(const char* filename = NULL);
    void _ParseLine(char* line);
    char* _TrimSpace(char* name); //裁剪需要的key或者value

    bool m_load_ok;
    map<string, string> m_config_map;
    string m_config_file;
};

#endif