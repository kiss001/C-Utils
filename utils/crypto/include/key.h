#pragma once

#include "buffer.h"

class Key : public Buffer {
public:    
    Key() {}
    Key(const Key& rhs) { copy(rhs.buf(), rhs.len()); }
    Key(const uint8_t* buf, uint32_t len) : Buffer(buf, len) {}
    Key(const char* str) : Buffer(str) {}

    ~Key() {}

private:

};