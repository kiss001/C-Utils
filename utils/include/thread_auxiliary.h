#pragma once 

#include <pthread.h>

namespace scss {

class ThreadAuxiliary {
public:
    static bool init_mutex_and_cond(pthread_mutex_t* mutex, pthread_cond_t* cond);
    static void uninit_mutex_and_cond(pthread_mutex_t* mutex, pthread_cond_t* cond);
    static pthread_t get_tid();
    static int get_thread_policy(pthread_attr_t *attr); 
    static void show_thread_priority(pthread_attr_t *attr,int policy);
    static int get_thread_priority(pthread_attr_t *attr);
    static void set_thread_policy(pthread_attr_t *attr,int policy);
    static bool create_thread(pthread_t* thread, void* (*svc)(void*), void* args, int priority); 
};

}
