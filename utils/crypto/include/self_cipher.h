#pragma once

#include "singleton.h"
#include "buffer.h"

class SelfCipher : public Singleton<SelfCipher> {
public:
    Buffer encrypt(const std::string& msg);

private:
    friend Singleton<SelfCipher>; 
    SelfCipher();
    ~SelfCipher();

    Buffer key_merge(const Buffer& key, const Buffer& buf);

};