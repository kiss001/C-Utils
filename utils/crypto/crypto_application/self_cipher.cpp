#include "self_cipher.h"

#include <assert.h>
#include <string.h>

#include "random.h"
#include "crypto_auxiliary.h"
#include "cipher.h"

SelfCipher::SelfCipher() {

}

SelfCipher::~SelfCipher() {

}

Buffer SelfCipher::key_merge(const Buffer& key, const Buffer& buf) {
    if (key.len() != 16) {
        assert(false);
    }

    int key_len = key.len();
    int buf_len = buf.len();
    int key_len_half = key_len / 2;

    uint8_t result[key_len + buf_len];
    uint8_t pre_key[key_len_half];
    uint8_t end_key[key_len_half];

    for (int i = 0; i < key_len_half; ++i) {
        pre_key[i] = key.at(i) ^ buf.at(i);
    }
    for (int i = 0; i < key_len_half; ++i) {
        end_key[i] = key.at(key_len_half + i) ^ buf.at(buf_len - key_len_half + i);
    }

    memcpy(result, pre_key, key_len_half);
    memcpy(result + key_len_half, buf.buf(), buf_len);
    memcpy(result + key_len_half + buf_len, end_key, key_len_half);

    return Buffer(result, key_len + buf_len);
}

Buffer SelfCipher::encrypt(const std::string& msg) {
    uint8_t key[16];
    Random::next_bytes(key, 16);

    Cipher* cipher = CryptoAuxiliary::instance()
        ->get_cipher_instance(E_CIPHER_ALGORITHM_AES_CBC_PKCS5PADDING);
    Key key_obj(key, 16);
    AlgorithmParam param(key, 16);
    cipher->init(E_CIPHER_MODE_ENCRYPT, key_obj, param);
    Buffer in(msg);
    Buffer out;
    if (cipher->do_final(in, out) != RTN_OK) {
        return Buffer();
    }
    delete cipher;

    return key_merge(key_obj, out);
}
