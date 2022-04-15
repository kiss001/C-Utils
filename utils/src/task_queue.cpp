#include "task_queue.h"

// namespace scss {

TaskQueue::TaskQueue()
    : _inited(false),
      _exitting(false),
      _concurrent(0) {
}

TaskQueue::~TaskQueue() {
    final();
}

bool TaskQueue::init() {
    assert(!_inited);

    int result = pthread_mutex_init(&_mutex, NULL);
    if (0 != result) {
        return false;
    }

    result = pthread_cond_init(&_cond, NULL);
    if (0 != result) {
        pthread_mutex_destroy(&_mutex);
        return false;
    }

    _inited = true;
    return true;
}

void TaskQueue::final() {
    if (_inited) {
        clear();
        pthread_cond_destroy(&_cond);
        pthread_mutex_destroy(&_mutex);
        _inited = false;
    }
}

Task* TaskQueue::poll() {
    assert(_inited);

    Task* task = NULL;
    pthread_mutex_lock(&_mutex);
    if (!_exitting && 0 == _task_queue.size()) {
        _concurrent++;
        pthread_cond_wait(&_cond, &_mutex);
        _concurrent--;
        assert(_concurrent >= 0);
    } 

    if (_task_queue.size() > 0) {
        task = _task_queue.front();
        _task_queue.pop_front();
    }

    pthread_mutex_unlock(&_mutex);
    return task;
}

void TaskQueue::push(Task* task) {
    assert(_inited);

    pthread_mutex_lock(&_mutex);
    _task_queue.push_back(task);
    if (_concurrent > 0) {
        pthread_cond_signal(&_cond);
    }
    pthread_mutex_unlock(&_mutex);
}

size_t TaskQueue::size() {
    pthread_mutex_lock(&_mutex);
    size_t size = _task_queue.size();
    pthread_mutex_unlock(&_mutex);

    return size;
}

void TaskQueue::decouple() {
    assert(_inited);

    pthread_mutex_lock(&_mutex);
    _exitting = true;
    pthread_cond_broadcast(&_cond);
    pthread_mutex_unlock(&_mutex);
}

bool TaskQueue::cancel(Task* task) {
    assert(_inited);

    bool flag = false;
    pthread_mutex_lock(&_mutex);
    for (std::list<Task*>::iterator it = _task_queue.begin(); it != _task_queue.end(); ++it) {
        if (*it == task) {
            task = *it;
            _task_queue.erase(it);
            flag = true;
            break;
        }
    }
    pthread_mutex_unlock(&_mutex);

    if (!flag) {
        return false;
    }

    task->cancel();
    delete task;
    return true;
}

bool TaskQueue::cancel() {
    clear();

    return true;
}

void TaskQueue::clear() {
    assert(_inited);

    std::list<Task*> tmp_queue;
    pthread_mutex_lock(&_mutex);
    swap(_task_queue, tmp_queue);
    pthread_mutex_unlock(&_mutex);

    Task* task = NULL;
    while (tmp_queue.size() > 0) {
        task = tmp_queue.front();
        tmp_queue.pop_front();
        task->cancel();
        delete task;
    }
}

// } /* namespace scss */
