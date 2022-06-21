#pragma once

#include "buffer.h"

#include <string>
#include <stdint.h>

class ByteUtils {
public:
    static bool equals(const Buffer& var0, const Buffer& var1);
    static std::string bin2hex(const uint8_t* buf, uint32_t len);
    static std::string bin2hex(const Buffer& buf);

    static bool hex2bin(const std::string& hex, Buffer& bin);

private:    

};