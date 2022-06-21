#include "openssl_provider.h"

#include "logger.h"
#include "openssl_cipher_engine.h"
#include "openssl_digest_engine.h"

OpenSSLProvider::OpenSSLProvider() {
    init();
}

OpenSSLProvider::~OpenSSLProvider() {
    uinit();
}


RtnCode OpenSSLProvider::init() {
    // 增加Cipher引擎
    OpenSSLCipherEngine* cipher_engine = new OpenSSLCipherEngine();
    add_engine("cipher", cipher_engine);

    // 增加Digest引擎
    OpenSSLDigestEngine* digest_engine = new OpenSSLDigestEngine();
    add_engine("digest", digest_engine);

    return RTN_OK;
}

void OpenSSLProvider::uinit() {

}

Cipher* OpenSSLProvider::get_cipher_instance(CipherAlgorithm name) {
    if (is_exist_engine("cipher")) {
        OpenSSLCipherEngine* cipher_engine = (OpenSSLCipherEngine*) get_engine("cipher");
        return cipher_engine->get_instance(name);
    }

    logi("The OpenSSL Provider cipher is not support\n");
    return NULL;
}

MessageDigest* OpenSSLProvider::get_digest_instance(MessageDigestMode name) {
    if (is_exist_engine("digest")) {
        OpenSSLDigestEngine* digest_engine = (OpenSSLDigestEngine*) get_engine("digest");
        return digest_engine->get_instance(name);
    }

    logi("The OpenSSL Provider digest is not support\n");
    return NULL;
}

