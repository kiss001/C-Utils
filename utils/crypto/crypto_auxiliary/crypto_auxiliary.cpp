#include "crypto_auxiliary.h"

#include "logger.h"

#define OPENSSL_PROVIDER

#ifdef OPENSSL_PROVIDER
#include "openssl_provider.h"
#endif

CryptoAuxiliary::CryptoAuxiliary() {
    _provider = NULL;

    init();
}

CryptoAuxiliary::~CryptoAuxiliary() {
    uinit();
}

RtnCode CryptoAuxiliary::init() {
    Provider* provider = NULL;

#ifdef OPENSSL_PROVIDER
    provider = new OpenSSLProvider();
#endif

    _provider = provider;
}

void CryptoAuxiliary::uinit() {
    if (_provider != NULL) {
        delete _provider;
        _provider = NULL;
    }   
}

Cipher* CryptoAuxiliary::get_cipher_instance(CipherAlgorithm cipher_algorithm) {
    return _provider->get_cipher_instance(cipher_algorithm);
}

MessageDigest* CryptoAuxiliary::get_digest_instance(MessageDigestMode digest_mode) {
    return _provider->get_digest_instance(digest_mode);
}
