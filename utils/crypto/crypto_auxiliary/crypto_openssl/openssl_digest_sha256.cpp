#include "openssl_digest_sha256.h"

#include "logger.h"

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

OpenSSLDigestSha256::OpenSSLDigestSha256() {

}

OpenSSLDigestSha256::~OpenSSLDigestSha256() {

}

std::string OpenSSLDigestSha256::openssl_error() {
    int error = ERR_get_error();
    return ERR_reason_error_string(error);
}

void OpenSSLDigestSha256::update(const Buffer& in) {
    _message = in;
}

RtnCode OpenSSLDigestSha256::digest(Buffer& out) {
    RtnCode rtn = RTN_OK;

    // 初始化上下文
    EVP_MD_CTX* mdctx = NULL;
	if((mdctx = EVP_MD_CTX_create()) == NULL) {
        rtn = ERROR_DIGEST_INIT_ERROR;

        loge("The EVP_MD_CTX_new faild, error %s\n", openssl_error().c_str());
        return rtn;
    }
    do {
        if(1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL)) {
            rtn = ERROR_DIGEST_INIT_ERROR;
            loge("The EVP_DigestInit_ex faild, error %s\n", openssl_error().c_str());
            break;
        }

        if(1 != EVP_DigestUpdate(mdctx, _message.buf(), _message.len())) {
            rtn = ERROR_DIGEST_ERROR;
            loge("The EVP_DigestUpdate faild, error %s\n", openssl_error().c_str());
            break;
        }
        
        unsigned char* digest = (unsigned char*)OPENSSL_malloc(EVP_MD_size(EVP_sha256()));
        if (NULL == digest) {
            rtn = ERROR_DIGEST_ERROR;
            loge("The OPENSSL_malloc faild, error %s\n", openssl_error().c_str());
            break;
        }

        unsigned int digest_len = 0;
        if(1 != EVP_DigestFinal_ex(mdctx, digest, &digest_len)) {
            OPENSSL_free(digest);

            rtn = ERROR_DIGEST_ERROR;
            loge("The EVP_DigestFinal_ex faild, error %s\n", openssl_error().c_str());
            break;
        }

        out = Buffer(digest, digest_len);
        
        OPENSSL_free(digest);
    } while(0);
	
	EVP_MD_CTX_destroy(mdctx);

    return rtn;
}