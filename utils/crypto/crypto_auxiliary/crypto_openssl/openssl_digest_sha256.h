#pragma once

#include "message_digest.h"

class OpenSSLDigestSha256 : public MessageDigest {
public:
    OpenSSLDigestSha256();
    ~OpenSSLDigestSha256();

    void update(const Buffer& in);
    RtnCode digest(Buffer& out); 

private:
    Buffer _message;
    
    std::string openssl_error();

};