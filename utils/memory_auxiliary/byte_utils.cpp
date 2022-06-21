#include "byte_utils.h"

#include "string.h"
#include "string_utils.h"

static std::string _alpha = "0123456789ABCDEF";

bool ByteUtils::equals(const Buffer& var0, const Buffer& var1) {
    if (var0.len() != var1.len()) {
        return false;
    }

    if (0 == memcmp(var0.buf(), var1.buf(), var0.len())) {
        return true;
    }

    return false;
}

std::string ByteUtils::bin2hex(const uint8_t* buf, uint32_t len) {
    int hex_len = 2 * len;
    char rtn_buf[hex_len];
    memset(rtn_buf, 0, hex_len);

    int buf_len = 0;
    for (int i = 0; i < len; ++i) {
        rtn_buf[buf_len++] = _alpha.at(buf[i] >> 4 & 0xf);
        rtn_buf[buf_len++] = _alpha.at(buf[i] & 0xf);
    }

    std::string str(rtn_buf, 2 * len);

    return str;
}

std::string ByteUtils::bin2hex(const Buffer& buf) {
    return ByteUtils::bin2hex(buf.buf(), buf.len());
}

bool ByteUtils::hex2bin(const std::string& hex, Buffer& bin) {
    std::string temp = hex;
    
    if (StringUtils::starts_with(temp, "0x")) {
        temp = temp.substr(2);
    }

    temp = StringUtils::to_upper(temp);
    int len = temp.length();

    uint8_t bin_buf[len];
    memset(bin_buf, 0, len);

    int index = 0;
    for (int i = 0; i < len; ++i) {
        if (!StringUtils::contains(_alpha, temp.at(i))) {
            return false;
        }

        if ('0' <= temp.at(i) <= '9') {
            bin_buf[index++] = temp.at(i) - '0';
        } else if ('A' <= temp.at(i) <= 'F') {
            bin_buf[index++] = temp.at(i) - 'A';
        }
    }

    bin = Buffer(bin_buf, len);

    return true;
}

