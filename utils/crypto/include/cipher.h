#pragma once

#include <string>

#include "key.h"
#include "algorithm_param.h"
#include "buffer.h"
#include "rtn_code.h"

enum CipherMode {
    E_CIPHER_MODE_MIN = 0,
    E_CIPHER_MODE_ENCRYPT = 1,
    E_CIPHER_MODE_DECRYPT = 2,
    E_CIPHER_MODE_MAX
};

enum CipherAlgorithm {
    E_CIPHER_ALGORITHM_MIN = 0,
    E_CIPHER_ALGORITHM_AES_CBC_NOPADDING,
    E_CIPHER_ALGORITHM_AES_CBC_PKCS5PADDING,
    E_CIPHER_ALGORITHM_AES_ECB_NOPADDING,
    E_CIPHER_ALGORITHM_AES_ECB_PKCS5PADDING,
    E_CIPHER_ALGORITHM_DES_CBC_NOPADDING,
    E_CIPHER_ALGORITHM_DES_CBC_PKCS5PADDING,
    E_CIPHER_ALGORITHM_DES_ECB_NOPADDING,
    E_CIPHER_ALGORITHM_DES_ECB_PKCS5PADDING,
    E_CIPHER_ALGORITHM_MAX
};

enum CipherGroupMode {
    E_CIPHER_GROUP_MODE_MIN = 0,
    E_CIPHER_GROUP_MODE_ECB,
    E_CIPHER_GROUP_MODE_CBC,
    E_CIPHER_GROUP_MODE_MAX
};

enum CipherPadding {
    E_CIPHER_PADDING_MIN = 0,
    E_CIPHER_PADDING_NOPADDING,
    E_CIPHER_PADDING_PKCS5,
    E_CIPHER_PADDING_PKCS7,
    E_CIPHER_PADDING_ZERO,
    E_CIPHER_PADDING_ISO7816_4,
    E_CIPHER_PADDING_ANSI923,
    E_CIPHER_PADDING_ISO10126,
    E_CIPHER_PADDING_MAX

};

class Cipher {
public:
    // 这些函数都可以被多次调用，非线程安全
    virtual void init(CipherMode mode, const Key& key) = 0;
    virtual void init(CipherMode mode, const Key& key, const AlgorithmParam& param) = 0;
    virtual RtnCode do_final(const Buffer& in, Buffer& out) = 0;

protected:

};