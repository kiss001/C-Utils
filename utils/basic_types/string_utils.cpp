#include "string_utils.h"


#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <libgen.h>

/**
 * @brief   当前仅仅支持#类型的注释
 * 
 * @param s 
 * @return std::string& 
 */
std::string& StringUtils::remove_comment(std::string &s) {
	if (s.empty()) {
        return s;
    }

    if (-1 != s.find_first_of("#")) {
        s.erase(s.find_first_of("#"));
    }

	return s;
}

/**
 * @brief 去掉首尾空格
 * 
 * @param s 
 * @return std::string& 
 */
std::string& StringUtils::trim(std::string &s) {
    if (s.empty()) {
        return s;
    }

    if (-1 != s.find_first_not_of(" ")) {
        s.erase(0, s.find_first_not_of(" "));
    }

    // if (-1 != s.find_last_not_of(" ")) {
    s.erase(s.find_last_not_of(" ") + 1);
    // }


    
    return s;
}

/**
 * @brief 按照单个字符切割字符串，利用C语言的stroke函数封装
 * 
 * @param str           需要切割的字符串
 * @param character     切割字符
 * @param vector        输出的字符串数组
 * @return boolean      函数执行结果
 */
boolean StringUtils::split(const std::string& str, const std::string delim, StringVector& vector) {
    assert(0 != str.length()); 

    //先将要切割的字符串从string类型转换为char*类型  
	char* str_chars = new char[str.length() + 1] ; 
	strcpy(str_chars, str.c_str());   
	char* d = new char[delim.length() + 1];  
	memset(d, 0, delim.length() + 1);
	memcpy(d, delim.c_str(), delim.size());  
    
    // 切割字符串
	char *p = strtok(str_chars, d);  
	while(p != NULL) {
        //分割得到的字符串转换为string类型  
		std::string split_str = p; 

        //存入结果数组 
		vector.push_back(split_str);

		p = strtok(NULL, d);  
	}

    // 释放内存
    delete str_chars;
    delete d;

    return true;
}

std::string StringUtils::to_upper(const std::string &str) {
    std::string rtn = "";

    for (int i = 0; i < str.size(); i++) {
        rtn += (char)toupper(str[i]);
    }

    return rtn;
}

std::string StringUtils::to_lower(const std::string &str) {
    std::string rtn = "";
    
    for (int i = 0; i < str.size(); i++) {
        rtn += (char)tolower(str[i]);
    }

    return rtn;
}

bool StringUtils::contains(const std::string& str, const std::string& var0) {
    if (str.find(var0) != -1) {
        return true;
    }

    return false;
}

bool StringUtils::contains(const std::string& str, const char& var0) {
    if (str.find(var0) != -1) {
        return true;
    }

    return false;
}

bool StringUtils::starts_with(const std::string& str, const std::string prefix) {
    int prefix_len = prefix.length();
    int str_len = str.length();

    assert(prefix_len > 0);

    int i = 0;
    for (; i < prefix_len && i < str_len; ++i) {
        if (prefix[i] != str.at(i)) {
            break;
        }
    }

    if (i == prefix_len) {
        return true;
    }

    return false;
}

std::string StringUtils::valueof(long num) {
    char str[1024];
    memset(str, 0, sizeof(str));

    sprintf(str, "%ld", num);

    return std::string(str);
}

std::string StringUtils::dir_name(const std::string& str) {
    if (str.empty()) {
        return str;
    }

    char temp[1024];
    memset(temp, 0, sizeof(temp));
    str.copy(temp, str.length());

    return dirname(temp);
}

std::string StringUtils::base_name(const std::string& str) {
    if (str.empty()) {
        return str;
    }

    char temp[1024];
    memset(temp, 0, sizeof(temp));
    str.copy(temp, str.length());

    return basename(temp);
}


// StringUtils::StringUtils() {

// }

// StringUtils::~StringUtils() {
    
// }

// StringUtils::StringUtils(const char* s) : std::string(s) {
    
// }

// StringUtils::StringUtils(std::string& str) : std::string(str) {

// }

// StringUtils::StringUtils (const StringUtils& str) : std::string(str.c_str()) {
    
// }

// StringUtils& StringUtils::operator=(const StringUtils &s) {
//     assign(s.c_str());

//     return *this;
// }

// StringUtils& StringUtils::operator=(const char* s) {
//     assign(s);

//     return *this;
// }

// StringUtils& StringUtils::operator=(const std::string& s) {
//     assign(s);

//     return *this;
// }

// StringUtils& StringUtils::operator=(char c) {
//     std::string temp;
//     temp = c;

//     assign(temp);

//     return *this;
// }

// StringUtils& StringUtils::operator+= (const StringUtils& str) {
//     append(str.c_str());

//     return *this;
// }

// StringUtils& StringUtils::operator+= (const std::string& str) {
//     append(str);

//     return *this;
// }

// StringUtils& StringUtils::operator+= (const char* s) {
//     append(s);

//     return *this;
// }

// StringUtils& StringUtils::operator+= (char c) {
//     std::string temp;
//     temp = c;

//     append(temp);

//     return *this;
// }


