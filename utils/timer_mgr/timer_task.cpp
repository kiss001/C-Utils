#include "timertask.h"

// namespace scss {

TimerTask::TimerTask(TimerHandler handler, void* param)
    : _handler(handler) {
    _param = param;
}

void TimerTask::run() {
    _handler(_param);
}

void TimerTask::cancel() {
}

// }
