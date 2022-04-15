#pragma once

#include "task.h"
#include "timer_mgr.h"
#include "types.h"
#include "rtn_code.h"

// namespace scss {

// typedef RtnCode (*TimerHandler)(void*);

class TimerTask : public Task {
public:
    TimerTask(TimerHandler handler, void* param = NULL);

    virtual void run();
    virtual void cancel();

private:
    TimerHandler _handler;
    
    void* _param;
};

// }

