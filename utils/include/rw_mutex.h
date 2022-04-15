#pragma once

#include <pthread.h>

// namespace utils {

class RwLocker {
public:
    inline RwLocker()        { pthread_rwlock_init(&rwlock,NULL); }
    inline ~RwLocker()       { pthread_rwlock_destroy(&rwlock); }
    inline void rdlock()     { pthread_rwlock_rdlock(&rwlock); }
    inline void wrlock()     { pthread_rwlock_wrlock(&rwlock); }
    inline void unlock()     { pthread_rwlock_unlock(&rwlock); }
private:
    pthread_rwlock_t rwlock;
};

// }
