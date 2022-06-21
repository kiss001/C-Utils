/**
 * @file task_thread.h
 * @author your name (you@domain.com)
 * @brief 线程，真正执行任务的线程
 * @version 0.1
 * @date 2022-06-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef BASE_THREADPOOL_TASKTHREAD_H
#define BASE_THREADPOOL_TASKTHREAD_H

#include <pthread.h>

#include "task_queue.h"

// namespace scss {

class TaskThread {
public:
    TaskThread();
    virtual ~TaskThread();

    bool init(TaskQueue* task_queue);
    bool create();
    void destroy();
    
    bool is_alive();
    void decouple();

protected:
    static  void* svc(void* arg);
    virtual void  svc();
    
    volatile bool _exitting;
    pthread_t     _thread;

    TaskQueue*  _task_queue;  // 线程不负责任务队列的释放
};

// } /* namespace scss */

#endif /* BASE_THREADPOOL_TASKTHREAD_H */
