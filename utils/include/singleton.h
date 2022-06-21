#pragma once

#include "mutex.h"

// namespace utils {

template <typename T>
class Singleton {
public:
    static T *instance() {
        if (_s_instance == NULL) {
            Locker::Autolock locker(&_s_mutex);
            if (_s_instance == NULL) {
                _s_instance = new T();
            }
        }
        return _s_instance;
    }
    static void release() {
        if (_s_instance != NULL) {
            delete _s_instance;
            _s_instance = NULL;
        }
    }

protected:
    Singleton() { }
    virtual ~Singleton() { }

private:
    Singleton(const Singleton &) { }
    Singleton &operator=(const Singleton &) { }
    static T *_s_instance;
    static Locker _s_mutex;
};

template <typename T>
T *Singleton<T>::_s_instance = NULL;

template<typename T>
Locker Singleton<T>::_s_mutex;

// }

