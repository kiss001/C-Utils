#ifndef BASE_INCLUDE_THREADPOOL_H
#define BASE_INCLUDE_THREADPOOL_H

#include "mutex.h"
#include "singleton.h"
#include "task_thread.h"
#include "types.h"

// namespace scss {

class Task;

class ThreadPool : public Singleton<ThreadPool> {
private:
    friend class Singleton<ThreadPool>;
    ThreadPool();
    ~ThreadPool();

    Task* pop();
    void uinit();

public:
    bool init(size_t core_size, TaskQueue* task_queue);
    
    void push(Task* task);

private:
    size_t          _core_thread_num;
    TaskThread*     _task_pool;

    TaskQueue*      _task_queue; 
};

// }

#endif /* BASE_INCLUDE_THREADPOOL_H */

