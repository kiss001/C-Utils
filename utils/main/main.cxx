#include <unistd.h>
#include <signal.h>

#include "svc_factory.h"
#include "app_args.h"
#include "config_mgr.h"
#include "logger.h"

using namespace std;

// 可执行的主体对象
static SvcController* _globla_instance = NULL;
static bool _stop_flag = false;

// 信号回调函数
void stop_svc(int signal) {
    _globla_instance->stop();

    _stop_flag = true;
}

int main(int argc, const char* argv[])  {
    RtnCode rtn = RTN_OK;

    // 初始化关键类
    do {
        if ((rtn = Logger::instance()->init(argc, argv)) != RTN_OK) {
            break;
        }

        if ((rtn = ApplicationArguments::instance()->init(argc, argv)) != RTN_OK) {
            break;
        }

        if ((rtn = ConfigMgr::instance()->init()) != RTN_OK) {
            break;
        }
    } while(0);
    
    if (rtn != RTN_OK) {
        return SERVICE_INIT_FAILED;
    }

    // 启动守护进程。不将当前目录设置为根目录，成功返回0
    // if (daemon(1, 0) < 0) {
    //     return -1;
    // }

    // 设置信号处理函数
    signal(SIGINT, stop_svc);

    // 创造执行对象
    _globla_instance = SvcFactory::instance()->create();
    assert(_globla_instance != NULL);
    
    // =================================================================
    // 功能执行主体
    // 1. 初始化
    rtn = _globla_instance->init();
    if (rtn != RTN_OK) {
        return SERVICE_INIT_FAILED;
    }

    // 2. 启动整个程序的运行
    (void)_globla_instance->start();

    // 3. 主进程循环
    do {
        struct timeval tv;
        tv.tv_sec = 3600;
        tv.tv_usec = 0;
        select(0, NULL, NULL, NULL, &tv);

        if (_stop_flag) {
            break;
        }
    } while(1);

    // 4. 去初始化
    (void)_globla_instance->uinit();

    // =================================================================
    // 释放资源
    SvcFactory::release();
    _globla_instance = NULL;

    ConfigMgr::release();
    ApplicationArguments::release();
    Logger::release();

    return rtn;
}