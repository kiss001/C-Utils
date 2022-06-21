/**
 * @file timer_mgr.h
 * @author your name (you@domain.com)
 * @brief  定时器管理，依赖线程池。它会调用线程池的单例。因此，不用显式的初始化线程池，但是如果需要定制化线程池，需要我们显式的初始化它。
 * @version 0.1
 * @date 2022-03-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include <stdint.h>
#include <map>
#include <sys/time.h>
#include "mutex.h"
#include "singleton.h"
#include "rtn_code.h"
#include "thread_pool.h"

// namespace scss {

class TimerTask;

typedef RtnCode (*TimerHandler)(void*);

// 每个应用程序自己进行定义，不放在这个工具库中
// typedef enum {
//     TTI_MIN = 0,
//     TTI_RULE_MGR_REQUEST_RULE, // 规则管理模块从云端获取安全规则
//     TTI_MAX,
// } TimerTaskId;

typedef struct {
    bool once;
    uint32_t interval_msec;
    uint32_t current_msec;
    TimerHandler handler;
    void* param;
} TimerTaskInfo;
 
typedef std::map<uint32_t, TimerTaskInfo*> TimerTaskMap;
 
class TimerMgr : 
    public Singleton<TimerMgr>, 
    public Locker, 
    public TimerTaskMap {

public: 
    TimerMgr();
    //return timertask_id
    uint32_t regist_timertask(uint32_t msec, bool once, TimerHandler handler, void* param);
    uint32_t regist_timertask(uint32_t timertask_id, uint32_t msec,
            bool once, TimerHandler handler, void* param);
    void unregist_timertask(uint32_t timertask_id);
    uint32_t search_timertask(uint32_t timertask_id);
    bool start();
    void stop();

private:
    friend class Singleton<TimerMgr>;
    ~TimerMgr();
    void on_timeout();
    uint32_t generate_ttid();

    static void* timer_thread(void*);

private:
    pthread_t _timer_tid;
    static bool _s_stop;
};

// }
