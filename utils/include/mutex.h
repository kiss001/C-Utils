#pragma once

#include <pthread.h>

// namespace utils {

class Locker {
    pthread_mutex_t mutex;
public:
    class Autolock {
        Locker* locker;
    public:
        inline explicit Autolock(Locker* locker) : locker(locker) {  locker->lock(); }
        inline ~Autolock() { locker->unlock(); }
    };

    inline Locker()        { pthread_mutex_init(&mutex, 0); }
    inline ~Locker()       { pthread_mutex_destroy(&mutex); }
    inline int tryLock()   { return pthread_mutex_trylock(&mutex); }
    inline void lock()     { pthread_mutex_lock(&mutex); }
    inline void unlock()   { pthread_mutex_unlock(&mutex); }
};

#define SYN_OBJ_EX(x) Locker::Autolock _syn_obj(x)
#define SYN_OBJ() Locker::Autolock _syn_obj(this)

// }
