#include "stringutils.h"

#include "string.h"

/**
 * @brief 按照单个字符切割字符串，利用C语言的stroke函数封装
 * 
 * @param str           需要切割的字符串
 * @param character     切割字符
 * @param vector        输出的字符串数组
 * @return boolean      函数执行结果
 */
boolean StringUtils::split(const String& str, const char delim, StringVector& vector) {
    assert(0 != str.length()); 

    //先将要切割的字符串从string类型转换为char*类型  
	char* str_chars = new char[str.length() + 1] ; 
	strcpy(str_chars, str.c_str());   
	char* d = new char[1];  
	d[0] = delim;  
    
    // 切割字符串
	char *p = strtok(str_chars, d);  
	while(p != NULL) {
        //分割得到的字符串转换为string类型  
		String split_str = p; 

        //存入结果数组 
		vector.push_back(split_str);

		p = strtok(NULL, d);  
	}

    // 释放内存
    delete str_chars;
    delete d;

    return true;
}