#ifndef BASE_INCLUDE_TASKQUEUE_H
#define BASE_INCLUDE_TASKQUEUE_H

#include <stdio.h>
#include <list>
#include <pthread.h>
#include <assert.h>
#include "task.h"

// namespace scss {

class TaskQueue {
public:
    TaskQueue();
    virtual ~TaskQueue();

    bool   init();
    void   final();

    Task*  poll();
    void   push(Task* task);
    size_t size();
    void   decouple();
    bool   cancel();
    bool   cancel(Task* task);

private:
    void   clear();

    bool             _inited;
    volatile bool    _exitting;
    volatile ssize_t _concurrent;
    pthread_mutex_t  _mutex;
    pthread_cond_t   _cond;
    std::list<Task*> _task_queue;
};

// } /* namespace scss */

#endif /* BASE_INCLUDE_TASKQUEUE_H */
