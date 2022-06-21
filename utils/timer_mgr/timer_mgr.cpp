#include "timer_mgr.h"

#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h> 
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#include "logger.h"
#include "task.h"
#include "timertask.h"
#include "task_queue.h"

// namespace scss {

bool TimerMgr::_s_stop = false;

TimerMgr::TimerMgr() {
    
}

TimerMgr::~TimerMgr() {
    TimerTaskMap::iterator iter = begin();
    while (iter != end()) {
        delete iter->second;
        erase(iter++);
    }
}

uint32_t TimerMgr::generate_ttid() {
    uint32_t random = rand() % 65535;

    while (1) {
        TimerTaskMap::iterator iter = find(random);
        if (iter == end()) {
            break;
        }

        random = rand() % 65535;
    }

    return random;
}

uint32_t TimerMgr::search_timertask(uint32_t timertask_id) {
    SYN_OBJ();

    TimerTaskMap::iterator iter = find(timertask_id);
    if (iter != end()) {
        return TIMER_ID_EXISTED;
    } 

    return RTN_OK;
}

uint32_t TimerMgr::regist_timertask(uint32_t msec, bool once, TimerHandler handler, void* param) {
    assert(0 != msec);

    TimerTaskInfo* ttinfo = NULL;
    SYN_OBJ();

    uint32_t timertask_id = generate_ttid();
    TimerTaskMap::iterator iter = find(timertask_id);
    if (iter == end()) {
        ttinfo = new TimerTaskInfo;
        insert(TimerTaskMap::value_type(timertask_id, ttinfo));
    } else {
        ttinfo = iter->second;
    }
    
    ttinfo->once = once;
    ttinfo->interval_msec = msec;
    ttinfo->current_msec = 0;

    // 只有两个定时器的回调函数相同才允许被覆盖
    assert(ttinfo->handler == handler);
    ttinfo->handler = handler;
    ttinfo->param = param;

    return timertask_id;
}

uint32_t TimerMgr::regist_timertask(uint32_t timertask_id, uint32_t msec
    , bool once, TimerHandler handler, void* param) {
    assert(0 != msec);

    TimerTaskInfo* ttinfo = NULL;
    SYN_OBJ();
    
    TimerTaskMap::iterator iter = find(timertask_id);
    if (iter == end()) {
        ttinfo = new TimerTaskInfo;
        insert(TimerTaskMap::value_type(timertask_id, ttinfo));
    } else {
        ttinfo = iter->second;
    }
    
    ttinfo->once = once;
    ttinfo->interval_msec = msec;
    ttinfo->current_msec = 0;
    ttinfo->handler = handler;
    ttinfo->param = param;

    return timertask_id;
} 

void TimerMgr::unregist_timertask(uint32_t ttid) {
    SYN_OBJ();
    TimerTaskMap::iterator iter = find(ttid);
    if (end() != iter) {
        delete iter->second;
        erase(iter);
    }
}

bool TimerMgr::start() {
    _s_stop = false;
    return (0 == pthread_create(reinterpret_cast<pthread_t*>(&_timer_tid), NULL, 
                &TimerMgr::timer_thread, NULL));
}

void TimerMgr::stop() {
    _s_stop = true;
    pthread_join(_timer_tid, NULL);
}

void* TimerMgr::timer_thread(void*) {
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
   
    while(1) {
        timeout.tv_sec = 0;
        timeout.tv_usec = 1000;
 
        int ret = select(0, NULL, NULL, NULL, &timeout);
        if (0 == ret) {
            if (_s_stop) {
                break;
            }
            TimerMgr::instance()->on_timeout();
        } else if (0 > ret) {
            loge("select server failed, errno is %d, errstr is %s\n", errno, strerror(errno));
            break;
        }
    }
    return NULL;
}

void TimerMgr::on_timeout() {
    SYN_OBJ();

    TimerTaskMap::iterator iter = begin();
    TimerTaskInfo* ttinfo = NULL;
    while (iter != end()) {
        ttinfo = iter->second;
        if (ttinfo->current_msec >= ttinfo->interval_msec) {
            Task* task = new TimerTask(ttinfo->handler, ttinfo->param);
            ThreadPool::instance()->push(task);

            if (ttinfo->once) {
                delete ttinfo;
                erase(iter++);
                continue;
            }
            ttinfo->current_msec = 0;
        }
        ttinfo->current_msec++;
        iter++;
    }
}

// }
