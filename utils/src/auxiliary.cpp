/*
 * auxiliary.cpp
 *
 *  Created on: Oct 11, 2017
 *      Author: paddle
 */
#include "auxiliary.h"

#include <unistd.h>
#include <dirent.h>
#include <netinet/in.h>
#include <net/if.h>  
#include <sys/ioctl.h>  
#include <sys/stat.h>
#include <sys/types.h>  
#include <sched.h>
#include <errno.h>
#include <string.h>
#include <fstream>
#include <assert.h>
#include "logger.h"

// namespace scss {
const char* Auxiliary::_s_hex_mark = "0123456789ABCDEF";
/*exemple: 
    format: abc/dec/hell/sdfd
    to vector: abc dec hell sdfd */
bool Auxiliary::split_string_by_substr(const std::string& format,
        const std::string& separator, std::vector<std::string>& params) {
    if (format.empty()) {
        return false;
    }

    std::string strs = format + separator;
    size_t pos = format.find(separator);
    size_t size = strs.size();

    while (pos != std::string::npos) {
        std::string x = strs.substr(0, pos);

        if (!x.empty()) {
            params.push_back(x);
        }

        strs = strs.substr(pos + 1, size);
        pos = strs.find(separator);
    }

    return true;
}

void Auxiliary::split_buf_by_char(std::vector<std::string>& v, const char *buf, size_t len, char splitchar) {
    uint8_t begin, end;
    size_t index = 0;
    while (index < len) {
        std::string str_ele = "";
        while (index < len && buf[index] == splitchar) {
            index++;
        }
        begin = index;
        end = begin;

        while (index < len && buf[index] != splitchar) {
            str_ele.append(1, buf[index]);
            index++;
        }
        end = index;

        if (end != begin) {
            v.push_back(str_ele);
        }
    }
}

bool Auxiliary::check_validity_of_hex(const char hex) {
    return ((hex >= '0' && hex <= '9') || (hex >= 'A' && hex <= 'F') || (hex >= 'a' && hex <= 'f'));
}

uint8_t Auxiliary::char2bin(const char c) {
    return (c & 15) + (c >> 6) * 9;
}

void Auxiliary::hex2bin(const char* md, size_t len, ByteArray& bin, size_t& bin_len) {
    assert(md != NULL || len != 0);
   
    bin_len = len >> 1;
    bin.reset(new uint8_t[bin_len]);
    for (size_t i = 0; i < bin_len; ++i) {
        char c = md[i * 2];
        assert(check_validity_of_hex(c));
        
        bin[i] = char2bin(c) << 4;
        c = md[i * 2 + 1];
        assert(check_validity_of_hex(c));
        
        bin[i] |= char2bin(c);
    }
}

void Auxiliary::hex2bin(const char* md, size_t len, Buffer& bin) {
    assert(md != NULL || len != 0);
   
    bin.alloc(len >> 1);
    uint8_t* pbuf = bin.buf();
    for (size_t i = 0; i < bin.len(); ++i) {
        char c = md[i * 2];
        assert(check_validity_of_hex(c));
        
        pbuf[i] = char2bin(c) << 4;
        c = md[i * 2 + 1];
        assert(check_validity_of_hex(c));
        
        pbuf[i] |= char2bin(c);
    }
}

void Auxiliary::bin2hex(const uint8_t* md, size_t len, std::string& str) {
    assert(md != NULL || len != 0);

    try {
        str.resize(len * 2);
    } catch (...) {
        return;
    }
    for (size_t i = 0; i < len; ++i) {
        str[i * 2] = hex2char((md[i] & 0xF0) >> 4);
        str[i * 2 + 1] = hex2char((md[i] & 0x0F));
    }
    return;
}

char Auxiliary::hex2char(const uint8_t hex) {
    if (hex > 0xF) {
        return 255;
    }
    return _s_hex_mark[hex];
}

void Auxiliary::uint2str(uint32_t uinteger, std::string& str) {
    char rslt[16];
    snprintf(rslt, sizeof(rslt), "%u", uinteger);
    str = std::string(rslt);
}

std::string Auxiliary::uint2str(uint32_t uinteger) {
    char rslt[16];
    snprintf(rslt, sizeof(rslt), "%u", uinteger);
    std::string str = std::string(rslt);
    return str;
}

bool Auxiliary::str2uint(const std::string& str, uint32_t base, uint32_t& uint) {
    assert(!str.empty());
    
    uint32_t rslt = 0;

    const char* pc = str.c_str();
    char c = 0;
    while ((c = *pc++) != 0) {
        bool valid = check_validity_of_hex(c);
        assert(valid);
        if (!valid) {
            return false;
        }
        rslt = rslt * base + char2bin(c);
    }
    
    uint = rslt;
    return true;
}

void Auxiliary::uint2buf_bigendin(uint32_t integer, uint8_t buf[sizeof(uint32_t)]) {
    for (uint32_t i = 0; i < sizeof(uint32_t); i++) {
        buf[sizeof(uint32_t) - i - 1] = (integer & 0xff);
        integer >>= 8;
    }
}

uint32_t Auxiliary::buf2uint_bigendin(uint8_t* buf, uint32_t buf_len) {
    if ((buf_len > sizeof(uint32_t)) || (buf_len == 0)) {
        return 0;
    }

    uint32_t integer = 0;
    for (uint32_t i = 0; i < buf_len; i++) {
        integer <<= 8;
        integer += buf[i];
    }
    return integer;
}

void Auxiliary::uint64_to_buf_bigendin(uint64_t u64, uint8_t* buf) {
    uint32_t i = 0;
    uint64_t tmp = u64;
    for (i = 7; i > 0; i--) {
        buf[i] = (tmp & 0x00000000000000ff);
        tmp = tmp >> 8;
    }
}

uint64_t Auxiliary::buf_to_uint64_bigendin(uint8_t* buf) {
    uint32_t i = 0;
    uint64_t u64 = 0;
    for (i = 0; i < sizeof(uint64_t); i++) {
        u64 = (u64 << 8) + buf[i];
    }
    return u64;
}
// } /* namespace scss */
