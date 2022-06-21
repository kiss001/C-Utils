/**
 * @file timertask.h
 * @author your name (you@domain.com)
 * @brief 这是一个定时器相关的任务，集成Task类
 * @version 0.1
 * @date 2022-06-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
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

