#pragma once

#include "cipher.h"
#include "rtn_code.h"

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

class OpenSSLCipherAES : public Cipher {
public:
    OpenSSLCipherAES(CipherGroupMode group_mode, CipherPadding padding);
    ~OpenSSLCipherAES();

    void init(CipherMode mode, const Key& key);
    void init(CipherMode mode, const Key& key, const AlgorithmParam& param);
    RtnCode do_final(const Buffer& in, Buffer& out);

private:
    // 公钥或者私钥
    Key _key;
    // 加密模式
    CipherMode _mode;
    // Padding模式
    CipherPadding _padding_mode;
    // 分组模式
    CipherGroupMode _group_mode;
    // IV向量等参数
    AlgorithmParam _param;
   
    RtnCode do_final_encrypt(const Buffer& in, Buffer& out);
    RtnCode do_final_decrypt(const Buffer& in, Buffer& out);

    EVP_CIPHER* get_evp_cipher();
    // 设置Padding，否则使用默认的方式
    void set_padding(EVP_CIPHER_CTX *ctx);

    std::string openssl_error();

};