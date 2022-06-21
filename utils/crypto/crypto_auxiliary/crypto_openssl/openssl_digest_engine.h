#pragma once

#include "engine.h"
#include "message_digest.h"

class OpenSSLDigestEngine : public Engine {
public:
    OpenSSLDigestEngine();
    ~OpenSSLDigestEngine();

    MessageDigest* get_instance(MessageDigestMode digest_mode);
    
};