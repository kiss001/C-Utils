#pragma once

#include "buffer.h"

class AlgorithmParam : public Buffer {
public:    
    AlgorithmParam() {}
    AlgorithmParam(const AlgorithmParam& rhs) { copy(rhs.buf(), rhs.len()); }
    AlgorithmParam(const uint8_t* buf, uint32_t len) : Buffer(buf, len) {}
    AlgorithmParam(const char* str) : Buffer(str) {}

    ~AlgorithmParam() {}

private:

};