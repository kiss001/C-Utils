#include "task_thread.h"
#include "task_queue.h"

// namespace scss {

#define XINVALID_THREAD (0)

TaskThread::TaskThread()
    : _exitting(false),
      _thread(XINVALID_THREAD) {
}

TaskThread::~TaskThread() {
    destroy();
}

bool TaskThread::init(TaskQueue* task_queue) {
    _task_queue = task_queue;
}

bool TaskThread::create() {
    assert(!is_alive());
    return pthread_create(&_thread, NULL, svc, this) == 0;
}

void TaskThread::destroy() {
    if (is_alive()) {
        void* tret = NULL;
        pthread_join(_thread, &tret);

        _exitting = false;
        _thread  = XINVALID_THREAD;
    }
}

bool TaskThread::is_alive() {
    return 0 == pthread_equal(_thread, XINVALID_THREAD);
}

void TaskThread::decouple() {
    _exitting = true;
}

void* TaskThread::svc(void* arg) {
    TaskThread* pthis = (TaskThread*)arg;
    pthis->svc();
    return NULL;
}

void TaskThread::svc() {
    assert(NULL != _task_queue);

    Task* task = NULL;
    while (!_exitting) {
        task = _task_queue->poll();
        if (NULL != task) {
            task->run();
            delete task;
        }
    }
}

// } /* namespace scss */