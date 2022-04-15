#pragma once

#include <stdint.h>
#include <vector>
#include <string>

#include "types.h"
#include "buffer.h"

// namespace scss {

class Auxiliary {
public:
    //group the string line with the separator
    //and places the all substring to the vector params
    static bool split_string_by_substr(const std::string& line, 
                const std::string& separator, std::vector<std::string>& params);

    //split the string by char and store the substring to vector v
    static void split_buf_by_char(std::vector<std::string>& v, 
                const char *buf, size_t len, char splitchar);

    static bool check_validity_of_hex(const char hex);
    static uint8_t char2bin(const char c);
    static void hex2bin(const char* md, size_t len, ByteArray& bin, size_t& bin_len);
    static void hex2bin(const char* md, size_t len, Buffer& bin);
    
    static void bin2hex(const uint8_t* md, size_t len, std::string& str);
    static char hex2char(const uint8_t hex);  

    static void uint2str(uint32_t uinteger, std::string& str); 
    static std::string uint2str(uint32_t uinteger); 
    static bool str2uint(const std::string& str, uint32_t base, uint32_t& uint);
    static void uint2buf_bigendin(uint32_t integer, uint8_t buf[sizeof(uint32_t)]); 
    static uint32_t buf2uint_bigendin(uint8_t* buf, uint32_t buf_len); 
    static void uint64_to_buf_bigendin(uint64_t u64, uint8_t* buf);
    static uint64_t buf_to_uint64_bigendin(uint8_t* buf);
private:
    static const char* _s_hex_mark;

};

// } /* namespace scss */

