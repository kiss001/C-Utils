/**
 * @file types.h
 * @author Guo qinglong (guoqinglong@baidu.com)
 * @brief 本文件主要定义一些基本的类型和一些基本的头文件包含
 * @version 0.1
 * @date 2022-01-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef UTILS_INCLUDE_TYPES_H
#define UTILS_INCLUDE_TYPES_H

#include <assert.h>
#include <stdint.h>  // 包含uint32,uint64等类型定义

#include <string>
#include <vector>

#include "unique_ptr.h"

typedef bool boolean;

typedef std::string String;
typedef std::vector<String> StringVector;

typedef UniquePtr<uint8_t[]> ByteArray;
typedef UniquePtr<uint32_t[]> WordArray;

#endif

