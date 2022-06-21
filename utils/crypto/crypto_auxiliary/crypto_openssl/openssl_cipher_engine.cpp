#include "openssl_cipher_engine.h"

#include "openssl_cipher_aes.h"
#include "logger.h"

OpenSSLCipherEngine::OpenSSLCipherEngine() {
}

OpenSSLCipherEngine::~OpenSSLCipherEngine() {
}

Cipher* OpenSSLCipherEngine::get_instance(CipherAlgorithm cipher_algorithm) {
    Cipher* cipher = NULL;

    // 每个算法都是一个独立的类实现，然后初始化算法的：运行模式和Padding
    switch (cipher_algorithm)
    {
    case E_CIPHER_ALGORITHM_AES_CBC_NOPADDING: {
        cipher = new OpenSSLCipherAES(E_CIPHER_GROUP_MODE_CBC, E_CIPHER_PADDING_NOPADDING);

        // 初始化参数

        break;
    }
    case E_CIPHER_ALGORITHM_AES_CBC_PKCS5PADDING: {
        cipher = new OpenSSLCipherAES(E_CIPHER_GROUP_MODE_CBC, E_CIPHER_PADDING_PKCS5);

        break;
    }
    case E_CIPHER_ALGORITHM_AES_ECB_NOPADDING: {
        cipher = new OpenSSLCipherAES(E_CIPHER_GROUP_MODE_ECB, E_CIPHER_PADDING_NOPADDING);

        break;
    }
    case E_CIPHER_ALGORITHM_AES_ECB_PKCS5PADDING: {
        cipher = new OpenSSLCipherAES(E_CIPHER_GROUP_MODE_ECB, E_CIPHER_PADDING_PKCS5);
        
        break;
    }
    case E_CIPHER_ALGORITHM_DES_CBC_NOPADDING: {
        break;
    }
    case E_CIPHER_ALGORITHM_DES_CBC_PKCS5PADDING: {
        break;
    }
    case E_CIPHER_ALGORITHM_DES_ECB_NOPADDING: {
        break;
    }
    case E_CIPHER_ALGORITHM_DES_ECB_PKCS5PADDING: {
        break;
    }
    default:
        break;
    }

    if (NULL == cipher) {
        logi("The Provider is not support type\n");
    }

    return cipher;
}

