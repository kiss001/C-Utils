#pragma once

#include "engine.h"
#include "cipher.h"

class OpenSSLCipherEngine : public Engine {
public:
    OpenSSLCipherEngine();
    ~OpenSSLCipherEngine();

    Cipher* get_instance(CipherAlgorithm cipher_algorithm);

private:

};