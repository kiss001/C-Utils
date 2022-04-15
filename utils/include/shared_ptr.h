#pragma once

#include <stdlib.h>  // For NULL.
#include <stdio.h>
#include <string>
#include <pthread.h>

// 注意：使用共享指针的循环引用的问题

// namespace utils {

// Default deleter for pointer types.
template <typename T>
struct DefaultDelete {
    enum { TYPE_MUST_BE_COMPLETE = sizeof(T) };
    DefaultDelete() {}
    void operator()(T* p) const { delete p; }
};

// Default deleter for array types.
template <typename T>
struct DefaultDelete<T[]> {
    enum { TYPE_MUST_BE_COMPLETE = sizeof(T) };
    void operator()(T* p) const { delete[] p; }
};

template <typename T, typename D = DefaultDelete<T> >
class Count {
public:
    explicit Count(T* ptr = NULL) : mPtr(ptr) {
        pthread_rwlock_init(&rwlock, NULL);

        s_num = 1;
    }

    ~Count() { 
        pthread_rwlock_destroy(&rwlock);

        reset(); 
    }

    void reset() {
        if (NULL != mPtr) {
            D()(mPtr);
            mPtr = NULL;
        }
    }

    T* release() {
        if (NULL != mPtr) {
            return mPtr;
        }

        return NULL;
    }

    T* get() {
        return mPtr;
    }

    int use_count() {
        return s_num;
    }

    void increase() {
        pthread_rwlock_wrlock(&rwlock);
        s_num += 1;
        pthread_rwlock_unlock(&rwlock);
    }

    void decrease() {
        pthread_rwlock_wrlock(&rwlock);
        s_num -= 1;
        pthread_rwlock_unlock(&rwlock);
    }

private:
    pthread_rwlock_t rwlock;

    T* mPtr;

    int s_num;  // shared_ptr引用计数

};

template <typename T, typename D >
class Count<T[], D> {
public:
    explicit Count(T* ptr = NULL) : mPtr(ptr) {
        pthread_rwlock_init(&rwlock, NULL);

        s_num = 1;
    }

    ~Count() { 
        pthread_rwlock_destroy(&rwlock);

        reset(); 
    }

    void reset() {
        if (NULL != mPtr) {
            D()(mPtr);
            mPtr = NULL;
        }
    }

    T* release() {
        if (NULL != mPtr) {
            return mPtr;
        }

        return NULL;
    }

    T* get() {
        return mPtr;
    }

    int use_count() {
        return s_num;
    }

    void increase() {
        pthread_rwlock_wrlock(&rwlock);
        s_num += 1;
        pthread_rwlock_unlock(&rwlock);
    }

    void decrease() {
        pthread_rwlock_wrlock(&rwlock);
        s_num -= 1;
        pthread_rwlock_unlock(&rwlock);
    }

private:
    pthread_rwlock_t rwlock;

    T* mPtr;

    int s_num;  // shared_ptr引用计数

};

template <typename T, typename D = DefaultDelete<T> >
class SharedPtr {
public:
    explicit SharedPtr(T* ptr = NULL) {
        _count = NULL;

        // printf("constructor\n");
        if (ptr == NULL) {
            return;
        }

        _count = new Count<T, D>(ptr);
    }

    ~SharedPtr() { release(); }

    // 获取智能指针的使用数
    int use_count() {
        if (NULL == _count) {
            return 0;
        }

        return _count->use_count();
    }

    // 判断智能指针是否是空智能指针
    bool isNull() {
        if (NULL == _count) {
            return true;
        }

        return false;
    }

    // 获取智能指针的内部对象，此函数不允许直接释放对象
    T* get() {
        if (NULL == _count) {
            return NULL;
        }

        return _count->get();
    }

    // 拷贝构造函数
    SharedPtr(const SharedPtr<T, D>& shared_ptr) {
        // printf("copy\n");
        if (this == &shared_ptr) {
            return;
        }

        _count = shared_ptr._count;
        if (NULL != _count) {
            _count->increase();
        }
    }

    // 赋值构造函数
    SharedPtr& operator=(const SharedPtr<T, D>& shared_ptr) {
        // printf("===\n");
        if (this == &shared_ptr) {
            return *this;
        }

        // 已经持有相同的指针，则赋值不生效
        if (shared_ptr._count == _count) {
            return *this;
        }

        if (NULL != _count) {
            _count->decrease();

            if (_count->use_count() <= 0) {
                delete _count;

                _count = NULL;
            }
        }

        _count = shared_ptr._count;

        if (NULL != _count) {
            _count->increase();
        }
        
        return *this;
    }
    
    // 智能指针的值比较，只要智能指针关联的指针相同，那么我们就认为这两个智能指针的值..相同
    // 慎重使
    bool operator==(const SharedPtr<T, D>& shared_ptr) {
        if (this == &shared_ptr) {
            return true;
        }

        if (_count == shared_ptr._count) {
            return true;
        }

        return false;
    }

private:
    Count<T, D>* _count;

    void release() {
        if (NULL != _count) {
            _count->decrease();

            if (_count->use_count() <= 0) {
                delete _count;

                _count = NULL;
            }
        }
    }
};

template <typename T, typename D >
class SharedPtr<T[], D> {
public:
    explicit SharedPtr(T* ptr = NULL) {
        _count = NULL;

        // printf("constructor array\n");
        if (ptr == NULL) {
            return;
        }

        _count = new Count<T[], D>(ptr);
    }

    ~SharedPtr() { release(); }

    int use_count() {
        if (NULL == _count) {
            return 0;
        }

        return _count->use_count();
    }

    bool isNull() {
        if (NULL == _count) {
            return true;
        }

        return false;
    }

    T* get() {
        if (NULL == _count) {
            return NULL;
        }

        return _count->get();
    }

    SharedPtr(const SharedPtr<T[], D>& shared_ptr) {
        // printf("copy array\n");
        if (this == &shared_ptr) {
            return;
        }

        _count = shared_ptr._count;
        if (NULL != _count) {
            _count->increase();
        }
    }

    SharedPtr& operator=(const SharedPtr<T[], D>& shared_ptr) {
        // printf("== array\n");
        if (this == &shared_ptr) {
            return *this;
        }
 
        if (shared_ptr._count == _count) {
            return *this;
        }

        if (NULL != _count) {
            _count->decrease();

            if (_count->use_count() <= 0) {
                delete _count;

                _count = NULL;
            }
        }

        _count = shared_ptr._count;

        if (NULL != _count) {
            _count->increase();
        }

        return *this;
    }

    // 智能指针的值比较，只要智能指针关联的指针相同，那么我们就认为这两个智能指针的值..相同
    // 慎重使用
    bool operator==(const SharedPtr<T[], D>& shared_ptr) {
        if (this == &shared_ptr) {
            return true;
        }

        if (_count == shared_ptr._count) {
            return true;
        }

        return false;
    }

private:
    Count<T[], D>* _count;
    
    void release() {
        if (NULL != _count) {
            _count->decrease();

            if (_count->use_count() <= 0) {
                delete _count;

                _count = NULL;
            }
        }
    }
};

// }

