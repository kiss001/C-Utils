/**
 * @file crypto_auxiliary.h
 * @author your name (you@domain.com)
 * @brief  整个程序的机构如下：CryptoAUxiliary --> Provider --> Engine  -->  Cipher等实现
 * @version 0.1
 * @date 2022-06-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include "singleton.h"
#include "rtn_code.h"
#include "cipher.h"
#include "provider.h"
#include "string_utils.h"
#include "message_digest.h"

class CryptoAuxiliary : public Singleton<CryptoAuxiliary> {
public:
    RtnCode init();
    void uinit();

    // 调用方需要手动释放内存
    Cipher* get_cipher_instance(CipherAlgorithm cipher_algorithm);
    MessageDigest* get_digest_instance(MessageDigestMode digest_mode);

private:
    friend Singleton<CryptoAuxiliary>;
    CryptoAuxiliary();
    ~CryptoAuxiliary();

    // 生命周期为整个生命周期
    Provider* _provider;

};