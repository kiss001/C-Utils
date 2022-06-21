#pragma once

#include <string.h>
#include <string>
#include <map>

#include "types.h"
#include "singleton.h"
#include "logger_impl.h"
#include "rtn_code.h"

// 日志类
class Logger : public Singleton<Logger> {
private:
    friend class Singleton<Logger>;
    Logger();
    ~Logger();

public:
    void log_print(LogLevel log_level, const char* format, ...);

    // 日志模块基本上会被所有的模块使用，因此日志模块需要在最开始就被初始化，当前日志类在命令行参数解析类初始化完成后立刻被初始化
    // 之所以放在第一个位置初始化，因为越放在后面导致约束就越来越多，不利于整个程序的编程。并且其他模块的信息也有必要记录到日志文件中
    // 放在第一个位置初始化，唯一的缺陷就是无法利用的其他模块提供的功能，但是相对而言需要的信息并不多
    // 日志模块初始化流程中，所有的打印都只能使用标准输出
    // ===============================================================================
    // 本函数配置文件路径的优先顺序：使用代码指定的日志路径 > 命令行--log_conf 指定的日志配置文件(不支持相对路径) > 日志配置默认路径config/log.conf
    // 日志配置文件最主要的配置为log_dir和log_level
    uint32_t init(int argc, const char* argv[], std::string log_conf_path = "");
    void uinit();

    // 仅供日志类初始化流程使用
    std::string get_config_by_key(std::string key);
    std::string get_work_dir();

private:
    // 具体日志实现类
    LoggerImpl* _logger_impl;

    // 配置项，以Key:Value的方式保存，Value统一存储为string，实际使用的时候做类型转换
    // 这里我们主要管理两项: "log_dir" "log_level"
    // 配置区分大小写
    std::map<std::string, std::string> _config_infos;

    // 工作目录
    std::string _work_dir_path;

    // 初始化函数
    void init_bin_dir_path();
    RtnCode init_log_config(std::string log_conf);

    void print_console(LogLevel log_level, const char* format, va_list args);

};

// sttchr此函数返回文件串中/符号出现的位置指针
#define FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1: __FILE__)

// 相关宏定义
#define loge(fmt, args...) \
do { \
    Logger::instance()->log_print(LOG_ERROR, "[%s] [%s] [%d] : " fmt, FILENAME, __FUNCTION__, __LINE__, ##args); \
} while (0);

#define logw(fmt, args...) \
do { \
    Logger::instance()->log_print(LOG_WARNING, "[%s] [%s] [%d] : " fmt, FILENAME, __FUNCTION__, __LINE__, ##args); \
} while (0);

#define logi(fmt, args...) \
do { \
    Logger::instance()->log_print(LOG_INFO, "[%s] [%s] [%d] : " fmt, FILENAME, __FUNCTION__, __LINE__, ##args); \
} while (0);

#define logd(fmt, args...) \
do { \
    Logger::instance()->log_print(LOG_DEBUG, "[%s] [%s] [%d] : " fmt, FILENAME, __FUNCTION__, __LINE__, ##args); \
} while (0);

#define logdc(dbg_ctrl, fmt, args...)\
do { \
    if (dbg_ctrl == 0) { \
        break; \
    } \
    log_print(LOG_DEBUG, "[%s] [%s] [%d] : " fmt, FILENAME, __FUNCTION__, __LINE__, ##args); \
} while (0);
