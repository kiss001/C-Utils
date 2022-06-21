/**
 * @file logger.cpp
 * @author your name (you@domain.com)
 * @brief 日志模块相当于一个最开始，最基础的模块。它自成模块，所以在初始化路径中，不能出现调用loge的调用
 * @version 0.1
 * @date 2022-06-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <libgen.h>
#include <assert.h>
#include <iostream>

#include "logger.h"
#include "file_auxiliary.h"
#include "string_utils.h"
#include "app_args.h"

#define GOOGLE_GLOG

#ifdef GOOGLE_GLOG
#include "logger_impl_google_glog.h"
#elif defined(ANDROID_LOGCAT)
#include "logger_impl_android_logcat.h"
#endif

Logger::Logger() {
    _logger_impl = NULL;

    init_bin_dir_path();
}

Logger::~Logger() {
    uinit();
}

std::string Logger::get_work_dir() {
    return _work_dir_path;
}

void Logger::init_bin_dir_path() {
    char buf[1024];
    // int count = readlink("/proc/pid/exe", buf, 1024);
    int count = readlink("/proc/self/exe", buf, 1024);
    if (count < 0 || count >= 1024 ) {
        std::cout << "readlink error: " << strerror(errno) << std::endl;

        assert(false);
    }
    buf[count] = '\0';

    // 获取可执行文件所在目录的父目录
    std::string _bin_dir_path = StringUtils::dir_name(buf);
    _work_dir_path = StringUtils::dir_name(_bin_dir_path);

}

std::string Logger::get_config_by_key(std::string key) {
    if (_config_infos.find(key) == _config_infos.end()) {

        return "";
    }

    return _config_infos[key];
}

RtnCode Logger::init_log_config(std::string log_conf) {
    RtnCode rtn = RTN_OK;

    do {
        if (log_conf.empty()) {
            break;
        }

        // 读取配置文件
        if (!FileAuxiliary::is_file_exist(log_conf)) {
            std::cout << "The log config file is not exist, " << log_conf << std::endl;
            rtn = ERROR_LOG_CONFIG_PARSE_FILED;
            break;
        }

        ByteArray buff;
        size_t size;
        if (!FileAuxiliary::read_file(log_conf, buff, size)) {
            std::cout << "Read log conf file failed, " << log_conf << std::endl;
            rtn = ERROR_LOG_CONFIG_PARSE_FILED;
            break;
        }

        std::string configs((char*)buff.get(), size);
        StringVector vector;
        if (!StringUtils::split(configs, "\n", vector)) {
            break;
        }

        for (int i = 0; i < vector.size(); ++i) {
            std::string line = vector.at(i);
            // 移除注释
            line = StringUtils::remove_comment(line);
            // 删除空格
            line = StringUtils::trim(line);
            if (line.empty()) {
                continue;
            }
            
            StringVector key_value;
            if (!StringUtils::split(line, "=", key_value) || (key_value.size() != 2)) {
                break;
            }

            std::string key = StringUtils::trim(key_value[0]);
            std::string values = StringUtils::trim(key_value[1]);

            _config_infos[key] = values;
        }
    } while (0);

    return rtn;
}

uint32_t Logger::init(int argc, const char* argv[], std::string log_conf_path) {
    // 第一，代码执行配置路径
    // 第二，命令行指定配置
    if (log_conf_path.empty()) {
        for (int i = 1; i < argc; i++) {
            const char* arg = argv[i];

            std::string arg_str = arg;  
            if (arg_str == "--log_conf=") {
                log_conf_path = arg_str.substr(11);
                break;
            }
        }

        // 第三，默认配置
        if (log_conf_path.empty()) {
            log_conf_path = get_work_dir() + "/config/log.conf";
        }
    }

    // 如果日志配置文件存在，读取日志配置文件
    if (RTN_OK != init_log_config(log_conf_path)) {
        return ERROR_LOG_CONFIG_PARSE_FILED;
    }
    
    // 第二，继续初始化日志具体实现类
    LoggerImpl* impl = NULL;

// 使用GOOGLE_GLOG作为日志输出
#if defined(GOOGLE_GLOG)
    impl = new LoggerImpleGoogleGlog("");
#elif defined(ANDROID_LOGCAT) 
    impl = new LoggerImpleLog();
#endif
    
    _logger_impl = impl;

    return 0;
}

void  Logger::uinit() {
    if (NULL != _logger_impl) {
        delete _logger_impl;
        _logger_impl = NULL;
    }
}

void Logger::print_console(LogLevel log_level, const char* format, va_list args) {
    char buf[1024];
    vsnprintf(buf, 1024, format, args);
    printf("%s", buf);
}

void Logger::log_print(LogLevel log_level, const char* format, ...) {
    va_list my_ap; 
	va_start(my_ap, format);  

    if (NULL != _logger_impl) {
        _logger_impl->print(log_level, format, my_ap);
    } else {
        print_console(log_level, format, my_ap);
    }

    va_end(my_ap); 
}