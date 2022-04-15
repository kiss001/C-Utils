#include "thread_pool.h"
#include "task_queue.h"

// namespace scss {

ThreadPool::ThreadPool() {
    _core_thread_num = 0;
    _task_pool = NULL;
    _task_queue = NULL;

    // 默认初始化
    init(5, new TaskQueue());
}

ThreadPool::~ThreadPool() {
    uinit();
}

bool ThreadPool::init(size_t core_size, TaskQueue* task_queue) {
    uinit();

    _task_queue = task_queue;

    // 设置核心线程数
    _core_thread_num = core_size;

    // 初始化线程
    _task_pool = new TaskThread[_core_thread_num];

    for (uint32_t i = 0; i < _core_thread_num; ++i) {
        _task_pool[i].init(_task_queue);

        if (!_task_pool[i].create()) {
            uinit();
            return false;
        }
    }

    return true;
}

void ThreadPool::uinit() {
    if (NULL != _task_queue) {
        delete _task_queue;
        _task_queue = NULL;
    }

    if (NULL != _task_pool) {
        // for (uint32_t i = 0; i < _core_thread_num; ++i) {
        //     _task_pool[i].decouple();
        // }

        // TaskQueue* task_queue = TaskQueue::instance();
        // assert(NULL != task_queue);
        // task_queue->decouple();
        
        for (uint32_t i = 0; i < _core_thread_num; ++i) {
            _task_pool[i].destroy();
        }

        delete[] _task_pool;
        _task_pool = NULL;
    }
}

void ThreadPool::push(Task* task) {
    assert(NULL != _task_queue);

    _task_queue->push(task);
}

Task* ThreadPool::pop() {
    assert(NULL != _task_queue);

    _task_queue->poll();
}



// } /* namespace scss */