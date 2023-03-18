#include <string>
namespace msfs {
    void replace_substrs(const char* search,
                        size_t search_len,
                        const char* replace,
                        size_t replace_len,
                        std::string* s);

//  用strncmp比较s1， s2；
    bool starts_with(const char* s1, const char* s2);

// 用s1的最后的位置和s2相比，比如s1:abcde, s2:cde,,s1必须比s2长
    bool ends_with(const char* s1, const char* s2);
    std::string string_trim(const std::string& s);

    //截取第一个. 出现前的字符串
    std::string string_prefix(const std::string&s, const char* sep = ".");// 前缀

    //截取最后一个. 出现之后的字符串
    std::string string_suffix(const std::string&s, const char* sep = ".");//后缀
}