#ifndef UTILS_INCLUDE_STRINGUTILS_H
#define UTILS_INCLUDE_STRINGUTILS_H

#include "types.h"

#include <string>

// namespace utils {

/**
 * @brief 包含字符串相关的一系列操作
 * 
 */
class StringUtils : public std::string
{
public:
    static boolean split(const std::string& str, const std::string delim, StringVector& vector);
    static std::string& trim(std::string &s);
    static std::string& remove_comment(std::string &s);
    static std::string to_upper(const std::string &str);
    static std::string to_lower(const std::string &str);
    static bool contains(const std::string& str, const std::string& var0);
    static bool contains(const std::string& str, const char& var0);
    static bool starts_with(const std::string& str, const std::string prefix);
    static std::string valueof(long num);

    // 之所以重载这两个函数是因为系统的dirname函数会修改原指针的内容。往往产生和期望不符的结果
    static std::string dir_name(const std::string& str);
    static std::string base_name(const std::string& str);

    // // 构造函数
    // StringUtils();
    // StringUtils(const char* s);
    // StringUtils(std::string& str);
    // StringUtils (const StringUtils& str);         // 拷贝构造函数
    // StringUtils& operator=(const StringUtils& s); // 赋值构造函数
    // StringUtils& operator=(const std::string& s); // 赋值构造函数
    // StringUtils& operator=(const char* s);   // 赋值构造函数
    // StringUtils& operator=(char c);          // 赋值构造函数

    // // 操作运算符重载
    // StringUtils& operator+= (const StringUtils& str);
    // StringUtils& operator+= (const std::string& str);
    // StringUtils& operator+= (const char* s);
    // StringUtils& operator+= (char c);

    // // 不可被继承此类
    // ~StringUtils();
    
private:

};

// }

#endif
