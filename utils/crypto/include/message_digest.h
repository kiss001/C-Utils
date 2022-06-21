#pragma once

#include "buffer.h"
#include "rtn_code.h"

#include <openssl/evp.h>

enum MessageDigestMode {
    E_MESSAGE_DIGEST_MODE_MIN = 0,
    E_MESSAGE_DIGEST_MODE_SHA256,
    E_MESSAGE_DIGEST_MODE_MAX
};

class MessageDigest {
public:
    virtual void update(const Buffer& in) = 0;
    virtual RtnCode digest(Buffer& out) = 0; 

private:

};