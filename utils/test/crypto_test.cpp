#include <gtest/gtest.h>

#include "crypto_auxiliary.h"
#include "cipher.h"
#include "byte_utils.h"
#include "message_digest.h"
#include "byte_utils.h"

#include <iostream>

TEST(Crypto, aes_256) {
    Cipher* cipher = CryptoAuxiliary::instance()
        ->get_cipher_instance(E_CIPHER_ALGORITHM_AES_CBC_PKCS5PADDING);

    const char *key = (char *)"01234567890123456789012345678901";
    const char *iv = (char *)"0123456789012345";
    const char *plaintext =
        (char *)"The quick brown fox jumps over the lazy dog";

    Key key_obj(key);
    AlgorithmParam parm(iv);
    Buffer input(plaintext);
    Buffer output;

    cipher->init(E_CIPHER_MODE_ENCRYPT, key_obj, parm);
    RtnCode rtn = cipher->do_final(input, output);
    ASSERT_TRUE(rtn == RTN_OK);

    std::string str = ByteUtils::bin2hex(output);
    
    std::cout << str << std::endl;

    cipher->init(E_CIPHER_MODE_DECRYPT, key_obj, parm);
    rtn = cipher->do_final(output, input);
    ASSERT_TRUE(rtn == RTN_OK);

    Buffer temp(plaintext);
    ASSERT_TRUE(ByteUtils::equals(input, temp));

    delete cipher;
}

TEST(Crypto, sha256) {
    MessageDigest* digest = CryptoAuxiliary::instance()
        ->get_digest_instance(E_MESSAGE_DIGEST_MODE_SHA256);
    ASSERT_TRUE(NULL != digest);

    char* input = (char*) "0123456789";
    Buffer in(input);
    digest->update(in);
    Buffer out;
    RtnCode rtn = digest->digest(out);
    ASSERT_TRUE(rtn == RTN_OK);
    ASSERT_TRUE(out.len() > 0);

    delete digest;
}

