#pragma once

#include <string.h>

#include "types.h"
#include "singleton.h"

// namespace utils {

#define LOG_LEVEL_NUM 4
enum LogLevel {
    LOG_ERROR = 0,
    LOG_WARNING,
    LOG_INFO,
    LOG_DEBUG,
};

// 日志类
class Logger : public Singleton<Logger> {
private:
    friend class Singleton<Logger>;
    Logger();
    ~Logger();

public:
    void log_print(LogLevel log_level, const char* format, ...);

private:
    std::string _module_name;
};

// sttchr此函数返回文件串中/符号出现的位置指针
#define FILENAME strrchr(__FILE__, '/')?strrchr(__FILE__, '/')+1:__FILE__

// 相关宏定义
#define loge(fmt, args...) \
do { \
    Logger::instance()->log_print(LOG_ERROR, "%s:%s:%d  " fmt, FILENAME, __FUNCTION__, __LINE__, ##args); \
} while (0);

#define logw(fmt, args...) \
do { \
    Logger::instance()->log_print(LOG_WARNING, "%s:%s:%d  " fmt, FILENAME, __FUNCTION__, __LINE__, ##args); \
} while (0);

#define logi(fmt, args...) \
do { \
    Logger::instance()->log_print(LOG_INFO, "%s:%s:%d  " fmt, FILENAME, __FUNCTION__, __LINE__, ##args); \
} while (0);

#define logd(fmt, args...) \
do { \
    Logger::instance()->log_print(LOG_DEBUG, "%s:%s:%d  " fmt, FILENAME, __FUNCTION__, __LINE__, ##args); \
} while (0);

#define logdc(dbg_ctrl, fmt, args...)\
do { \
    if (dbg_ctrl == 0) { \
        break; \
    } \
    log_print(LOG_DEBUG, "%s:%s:%d  " fmt, FILENAME, __FUNCTION__, __LINE__, ##args); \
} while (0);

// }