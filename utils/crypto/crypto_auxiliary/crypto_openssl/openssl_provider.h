#pragma once

#include "provider.h"
#include "cipher.h"
#include "rtn_code.h"
#include "message_digest.h"

class OpenSSLProvider : public Provider {
public: 
    OpenSSLProvider();
    ~OpenSSLProvider();

    RtnCode init();
    void uinit();

    // 只有注册引擎后重载这些函数才有意义
    Cipher* get_cipher_instance(CipherAlgorithm name);
    MessageDigest* get_digest_instance(MessageDigestMode name);

private:

};