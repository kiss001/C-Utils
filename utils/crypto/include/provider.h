#pragma once    

#include <map>
#include <string>

#include "cipher.h"
#include "engine.h"
#include "string_utils.h"
#include "message_digest.h"

class Provider {
public:
    Provider() {}
    virtual ~Provider() {
        std::map<std::string, Engine*>::iterator iter = _engines.begin();
        for (; iter != _engines.end(); ++iter) {
            delete iter->second;
        }
        _engines.clear(); 
    }

    // provider 提供一个符合条件的cipher实例
    virtual Cipher* get_cipher_instance(CipherAlgorithm name) {
        return NULL;
    }

    virtual MessageDigest* get_digest_instance(MessageDigestMode name) {
        return NULL;
    }

private:
    std::map<std::string, Engine*> _engines;

protected:
    void add_engine(std::string name, Engine* engine) { 
        name = StringUtils::to_lower(name); 
        _engines[name] = engine; 
    }
    
    bool is_exist_engine(std::string name) {
        name = StringUtils::to_lower(name);
    
        if (_engines.find(name) != _engines.end()) {
            return true;
        }

        return false;
    }

    Engine* get_engine(std::string name) {
        name = StringUtils::to_lower(name);
    
        if (_engines.find(name) != _engines.end()) {
            return _engines[name];
        }

        return NULL;
    }
};