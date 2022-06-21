#include "openssl_cipher_aes.h"

#include "logger.h"

OpenSSLCipherAES::OpenSSLCipherAES(CipherGroupMode group_mode, CipherPadding padding) {
    _group_mode = group_mode;
    _padding_mode = padding;
}

OpenSSLCipherAES::~OpenSSLCipherAES() {

}

void OpenSSLCipherAES::init(CipherMode mode, const Key& key) {
    _mode = mode;
    _key = key;
}

void OpenSSLCipherAES::init(CipherMode mode, const Key& key,const AlgorithmParam& param) {
    _mode = mode;
    _key = key;
    _param = param;
}

std::string OpenSSLCipherAES::openssl_error() {
    int error = ERR_get_error();
    return ERR_reason_error_string(error);
}

RtnCode OpenSSLCipherAES::do_final(const Buffer& in, Buffer& out) {
    RtnCode rtn = RTN_OK;

    switch(_mode) {
    case E_CIPHER_MODE_ENCRYPT: {
        return do_final_encrypt(in, out);
    }
    case E_CIPHER_MODE_DECRYPT: {
        return do_final_decrypt(in, out);
    }
    default: {
        break;
    }
    }

    return rtn;
}

EVP_CIPHER* OpenSSLCipherAES::get_evp_cipher() {
    const EVP_CIPHER* cipher = NULL;

    int key_len = _key.len() * 8;
    switch (key_len)
    {
    case 128: {
        if (_group_mode == E_CIPHER_GROUP_MODE_CBC) {
            cipher = EVP_aes_128_cbc();
        } else if (_group_mode == E_CIPHER_GROUP_MODE_ECB) {
            cipher = EVP_aes_128_ecb();
        }
        break;
    }
    case 192: {
        if (_group_mode == E_CIPHER_GROUP_MODE_CBC) {
            cipher = EVP_aes_192_cbc();
        } else if (_group_mode == E_CIPHER_GROUP_MODE_ECB) {
            cipher = EVP_aes_192_ecb();
        }
        break;
    }
    case 256: {
        if (_group_mode == E_CIPHER_GROUP_MODE_CBC) {
            cipher = EVP_aes_256_cbc();
        } else if (_group_mode == E_CIPHER_GROUP_MODE_ECB) {
            cipher = EVP_aes_256_ecb();
        }
        break;
    }
    default:
        break;
    }

    return const_cast<EVP_CIPHER*>(cipher);
}

void OpenSSLCipherAES::set_padding(EVP_CIPHER_CTX *ctx) {
    RtnCode rtn = RTN_OK;

    switch (_padding_mode)
    {
    case E_CIPHER_PADDING_NOPADDING: {
        EVP_CIPHER_CTX_set_padding(ctx, EVP_CIPH_NO_PADDING);
        break;
    }
    case E_CIPHER_PADDING_PKCS5: {
        EVP_CIPHER_CTX_set_padding(ctx, EVP_PADDING_PKCS7);
        break;
    }
    case E_CIPHER_PADDING_PKCS7: {
        EVP_CIPHER_CTX_set_padding(ctx, EVP_PADDING_PKCS7);
        break;
    }
    case E_CIPHER_PADDING_ZERO: {
        EVP_CIPHER_CTX_set_padding(ctx, EVP_PADDING_ZERO);
        break;
    }
    case E_CIPHER_PADDING_ISO7816_4: {
        EVP_CIPHER_CTX_set_padding(ctx, EVP_PADDING_ISO7816_4);
        break;
    }
    case E_CIPHER_PADDING_ANSI923: {
        EVP_CIPHER_CTX_set_padding(ctx, EVP_PADDING_ANSI923);
        break;
    }
    case E_CIPHER_PADDING_ISO10126: {
        EVP_CIPHER_CTX_set_padding(ctx, EVP_PADDING_ISO10126);
        break;
    }
    
    default:
        break;
    }
}

RtnCode OpenSSLCipherAES::do_final_encrypt(const Buffer& in, Buffer& out) {
    RtnCode rtn = RTN_OK;

    // 获取EVP_CIPHER
    EVP_CIPHER* cipher = get_evp_cipher();
    if (NULL == cipher) {
        rtn = ERROR_CRYPTO_INIT_ERROR;
        loge("The EVP_CIPHER init error, please check key_len or work_mode\n");
        return rtn;
    }

    // 创建上下文
    EVP_CIPHER_CTX* ctx = NULL;
    if(!(ctx = EVP_CIPHER_CTX_new())) {
        free(cipher);

        rtn = ERROR_CRYPTO_INIT_ERROR;
        loge("The EVP_CIPHER_CTX_new failed, error %s\n", openssl_error().c_str());
        return rtn;
    }

    // 执行操作
    do {
        if(1 != EVP_EncryptInit_ex(ctx
            , cipher
            , NULL
            , (unsigned char*)_key.buf()
            , (unsigned char*)_param.buf())) {
            rtn = ERROR_CRYPTO_INIT_ERROR;
            loge("The EVP_EncryptInit_ex failed, error %s\n", openssl_error().c_str());
            break;
        }

        // 设置Padding,应该在init调用之后调用
        set_padding(ctx);

        // 计算Output的最大长度
        Buffer output((in.len() / _key.len() + 1) * _key.len());

        int out_len = 0;
        if(1 != EVP_EncryptUpdate(ctx, output.buf(), &out_len, in.buf(), in.len())) {
            rtn = ERROR_CRYPTO_ENCRYPT_ERROR;
            loge("The EVP_EncryptUpdate failed, error %s\n", openssl_error().c_str());
            break;
        }

        /*
        * Finalise the encryption. Further ciphertext bytes may be written at
        * this stage.
        */
        int out_len2 = 0;
        if (1 != EVP_EncryptFinal_ex(ctx, output.buf() + out_len, &out_len2)) {
            rtn = ERROR_CRYPTO_ENCRYPT_ERROR;
            loge("The EVP_EncryptUpdate failed, error %s\n", openssl_error().c_str());
            break;
        }

        // assert((out_len + out_len2) == output.len());
        out = Buffer(output.buf(), out_len + out_len2);
    } while(0);

    /* 清理上下文 */
    EVP_CIPHER_CTX_free(ctx);

    return rtn;
}

RtnCode OpenSSLCipherAES::do_final_decrypt(const Buffer& in, Buffer& out) {
    RtnCode rtn = RTN_OK;

    // 获取EVP_CIPHER
    EVP_CIPHER *cipher = get_evp_cipher();
    if (NULL == cipher) {
        rtn = ERROR_CRYPTO_INIT_ERROR;
        loge("The EVP_CIPHER init error, please check key_len or work_mode\n");
        return rtn;
    }

    // 参数检查
    // if (_group_mode == E_CIPHER_GROUP_MODE_CBC) {
    //     if (_key.len() != 16) {
    //         free(cipher);

    //         rtn = ERROR_CRYPTO_PARAM_ERROR;
    //         loge("The CBC Group Mode don't hava iv\n");
    //         return rtn;
    //     }
    // }

    // 创建上下文
    EVP_CIPHER_CTX *ctx;
    if(!(ctx = EVP_CIPHER_CTX_new())) {
        free(cipher);

        rtn = ERROR_CRYPTO_INIT_ERROR;
        loge("The EVP_CIPHER_CTX_new failed, error %s\n", openssl_error().c_str());
        return rtn;
    }

    // 执行操作
    do {
        if(1 != EVP_DecryptInit_ex(ctx
            , cipher
            , NULL
            , (unsigned char *)_key.buf()
            , (unsigned char *)_param.buf())) {
            rtn = ERROR_CRYPTO_INIT_ERROR;
            loge("The EVP_DecryptInit_ex failed, error %s\n", openssl_error().c_str());
            break;
        }

        // 设置Padding,应该在init调用之后调用
        set_padding(ctx);

        Buffer output = Buffer(in.len());

        int out_len = 0;
        if(1 != EVP_DecryptUpdate(ctx, output.buf(), &out_len, in.buf(), in.len())) {
            rtn = ERROR_CRYPTO_DECRYPT_ERROR;
            loge("The EVP_DecryptUpdate failed, error %s\n", openssl_error().c_str());
            break;
        }

        /*
        * Finalise the encryption. Further ciphertext bytes may be written at
        * this stage.
        */
        int out_len2 = 0;
        if (1 != EVP_DecryptFinal_ex(ctx, output.buf() + out_len, &out_len2)) {
            rtn = ERROR_CRYPTO_DECRYPT_ERROR;
            loge("The EVP_DecryptFinal_ex failed, error %s\n", openssl_error().c_str());
            break;
        }

        // 进行padding操作
        out = Buffer(output.buf(), out_len + out_len2);
    } while(0);

    /* 清理上下文 */
    EVP_CIPHER_CTX_free(ctx);

    return rtn;
}


