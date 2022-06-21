#include "buffer.h"
#include <string.h>

#include <assert.h>

// namespace scss {

Buffer::Buffer() {
    init();
}

Buffer::Buffer(uint32_t len) {
    init();
    alloc(len);
}

// 对于字符串，buffer仅仅保存有效的长度，不包括\n
Buffer::Buffer(const char* str) {
    init();
    copy(reinterpret_cast<const uint8_t*>(str), strlen(str));
}

Buffer::Buffer(const Buffer& rhs) {
    init();
    copy(rhs.buf(), rhs.len());
}

Buffer::Buffer(const Buffer* rhs) {
    init();
    copy(rhs->buf(), rhs->len());
}

Buffer::Buffer(const uint8_t* buf, uint32_t len) {
    init();
    copy(buf, len);
}

Buffer::Buffer(const std::string& str) {
    init();
    copy(reinterpret_cast<uint8_t*>(const_cast<char*>(str.c_str())), str.length());
}

#if defined(C_PLUS_PLUS_11)
Buffer::Buffer(Buffer&& rhs) {
    _buf = rhs.detach(&_len);
}
#endif

Buffer::~Buffer() {
    free();
}

uint8_t& Buffer::operator[](uint32_t i) {
    assert(i < _len);

    return _buf[i];
}

Buffer& Buffer::operator=(const Buffer& rhs) {
    if (this != &rhs) {
        copy(rhs.buf(), rhs.len()); 
    }
    return *this;
}

#if defined(C_PLUS_PLUS_11)
Buffer& Buffer::operator=(Buffer&& rhs) {
    if (this != &rhs) {
        free();
        _buf = rhs.detach(&_len);
    }
    return *this;
}
#endif

void Buffer::alloc(uint32_t len) {
    free();
    
    _len = len;
    _buf = new uint8_t[len + 1];
    _buf[len] = 0;
}

void Buffer::copy(const uint8_t* buf, uint32_t len) {
    if (_buf == buf) {
        return;
    }
    free();

    if (0 == len) {
        return;
    }

    alloc(len);
    memcpy(_buf, buf, _len);
}

void Buffer::copy(const std::string& str) {
    free();

    alloc(str.length());
    memcpy(_buf, str.c_str(), _len);
}

void Buffer::attach(const uint8_t* buf, uint32_t len) {
    free();
    _buf = const_cast<uint8_t*>(buf);
    _len = len;
}

void Buffer::attach(const std::string& str) {
    free();
    _buf = const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(str.c_str()));
    _len = str.length();
}

uint8_t* Buffer::detach(uint32_t* len) {
    uint8_t* tmp = _buf;
    if (nullptr != len) {
        *len = _len;
    }
    
    init();
    return tmp;
}

uint8_t* Buffer::buf() const {
    return _buf;
}

uint8_t Buffer::at(uint32_t i) const {
    assert(i < _len);
    return _buf[i];
}

uint32_t Buffer::len() const {
    return _len;
}

uint32_t Buffer::clear() {
    return _buf == nullptr && _len == 0;
}

void Buffer::init() {
    _buf = nullptr;
    _len = 0;
}

void Buffer::free() {
    delete[] _buf;
    init();
}

// }
