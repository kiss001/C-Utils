#pragma once 

#include <iostream> 
// #include "object.h"
#include <stdint.h>

// namespace scss {

#define nullptr NULL

class Buffer {
public: 
    Buffer();
    virtual ~Buffer(); 
    
    // 构造函数
    explicit Buffer(uint32_t len);
    explicit Buffer(const char* str);
    Buffer(const Buffer& rhs);
    Buffer(const Buffer* rhs);
    Buffer(const uint8_t* buf, uint32_t len);
    Buffer(const std::string& str);
#if defined(C_PLUS_PLUS_11)
    explicit Buffer(Buffer&& rhs);
#endif

    // 运算符重载
    Buffer& operator=(const Buffer& rhs);

    // 允许通过下标运算符修改值
    uint8_t& operator[](uint32_t i);

#if defined(C_PLUS_PLUS_11)
    Buffer& operator=(Buffer&& rhs);
#endif

    uint8_t at(uint32_t i) const;
    void alloc(uint32_t len);
    void copy(const uint8_t* buf, uint32_t len); 
    void copy(const std::string& str); 
    void attach(const uint8_t* buf, uint32_t len);
    void attach(const std::string& str); 
    uint8_t* detach(uint32_t* len);
    uint8_t* buf() const;
    uint32_t len() const;
    uint32_t clear();
   
protected:
    virtual void init();
    virtual void free();

private:
    uint8_t* _buf;
    uint32_t _len;
};

// }
