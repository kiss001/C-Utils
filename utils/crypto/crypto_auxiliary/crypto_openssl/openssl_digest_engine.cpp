#include "openssl_digest_engine.h"

#include "logger.h"
#include "openssl_digest_sha256.h"

OpenSSLDigestEngine::OpenSSLDigestEngine() {

}

OpenSSLDigestEngine::~OpenSSLDigestEngine() {

}

MessageDigest* OpenSSLDigestEngine::get_instance(MessageDigestMode digest_mode) {
    MessageDigest* digest = NULL;

    switch (digest_mode)
    {
    case E_MESSAGE_DIGEST_MODE_SHA256: {
        digest = new OpenSSLDigestSha256();
        
        break;
    }
    default:
        break;
    }

    if (NULL == digest) {
        logi("The Provider is not support type\n");
    }

    return digest;
}