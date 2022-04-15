#include "thread_auxiliary.h"
#include <assert.h>
#include <errno.h>
#include "logger.h"

namespace scss {

bool ThreadAuxiliary::init_mutex_and_cond(pthread_mutex_t* mutex, pthread_cond_t* cond) {
    int result = pthread_mutex_init(mutex, NULL);
    if (0 != result) {
        loge("init mutex failed\n");
        return false;
    }

    result = pthread_cond_init(cond, NULL);
    if (0 != result) {
        loge("init cond failed\n");
        pthread_mutex_destroy(mutex);
        return false;
    }
    return true;
}

void ThreadAuxiliary::uninit_mutex_and_cond(pthread_mutex_t* mutex, pthread_cond_t* cond) {
    pthread_mutex_destroy(mutex);
    pthread_cond_destroy(cond);
}

pthread_t ThreadAuxiliary::get_tid() {
    return pthread_self();
}

int ThreadAuxiliary::get_thread_policy(pthread_attr_t *attr) {
    int policy = 0;
    int rs = pthread_attr_getschedpolicy(attr, &policy);
    if (rs != 0) {
        return -1;
    }
    
    switch(policy) {
        case SCHED_FIFO:
            logi("policy= SCHED_FIFO\n");
            break;
        case SCHED_RR:
            logi("policy= SCHED_RR\n");
            break;
        case SCHED_OTHER:
            logi("policy = SCHED_OTHER\n");
            break;
        default:
            logi("policy = UNKNOWN\n");
            break;
    }
    return policy;
}

void ThreadAuxiliary::show_thread_priority(pthread_attr_t *attr, int policy) {
    int priority = sched_get_priority_max(policy);
    assert(priority != -1);
    logi("max_priority= %d\n", priority);
    
    priority = sched_get_priority_min(policy);
    assert(priority != -1);
    logi("min_priority= %d\n", priority);
}

int ThreadAuxiliary::get_thread_priority(pthread_attr_t *attr) {
    struct sched_param param;
    int rs = pthread_attr_getschedparam(attr, &param);
    if (rs != 0) {
        return -1;
    }

    logi("priority=%d", param.__sched_priority);
    return param.__sched_priority;
}

void ThreadAuxiliary::set_thread_policy(pthread_attr_t *attr,int policy) {
    int rs = pthread_attr_setschedpolicy(attr, policy);
    if (rs != 0) {
        return;
    }
    
    get_thread_policy(attr);
}

bool ThreadAuxiliary::create_thread(pthread_t* thread, void* (*svc)(void*), void* args, int priority) {
    pthread_attr_t attr;

    int ret = pthread_attr_init(&attr);
    if (ret != 0) {
        return false;
    }
   
    do { 
        struct sched_param param;
        param.sched_priority = priority;

        ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);  
        if (0 != ret) {
            break;
        }
        ret = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
        if (0 != ret) {
            break;
        }
        
        ret = pthread_attr_setschedparam(&attr, &param);
        if (0 != ret) {
            break;
        }

        ret = pthread_create(thread, &attr, svc, args);
        if (0 != ret) {
            loge("pthread_create falied, errno is %d, error is %s, ret is %d\n", 
                    errno, strerror(errno), ret);
            break;
        }
    } while (0); 

    pthread_attr_destroy(&attr);
    return ret == 0;
}

}
