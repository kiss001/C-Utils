/**
 * @file logger_impl_google_glog.cpp
 * @author your name (you@domain.com)
 * @brief 参考：https://github.com/google/glog/tree/v0.6.0#custom-log-prefix-format
 * @version 0.1
 * @date 2022-06-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "logger_impl_google_glog.h"

#include <iostream>
#include <string>
#include <iomanip>

// #include <glog/logging.h>
#include "glog/logging.h"
#include "logger.h"

using namespace google;

void PrefixAttacher(std::ostream &s, const LogMessageInfo &l, void* data) {
    s << '-'
        << std::setw(4) << 1900 + l.time.year() << '-'
        << std::setw(2) << 1 + l.time.month() << '-'
        << std::setw(2) << l.time.day()
        << ' '
        << std::setw(2) << l.time.hour() << ':'
        << std::setw(2) << l.time.min()  << ':'
        << std::setw(2) << l.time.sec() << "."
        << std::setw(6) << l.time.usec() << ' '
        << '-' << l.severity << ' '
        << l.thread_id << std::setfill('0') << ' '
        << std::setw(4) << std::setfill('-');
}

LoggerImpleGoogleGlog::LoggerImpleGoogleGlog(std::string module_name) {
    _module_name = module_name;

    init();
}

LoggerImpleGoogleGlog::~LoggerImpleGoogleGlog() {
    uinit();
}

void LoggerImpleGoogleGlog::print(LogLevel log_level, const char* format, va_list args) {
    char buf[1024];
    vsnprintf(buf, sizeof(buf), format, args);
    
    switch (log_level)
    {
    case LOG_DEBUG: {
        DLOG(INFO) << buf;
        break;
    }
    case LOG_INFO: {
        LOG(INFO) << buf;
        break;
    }
    case LOG_WARNING: {
        LOG(WARNING) << buf;
        break;
    }
    case LOG_ERROR: {
        LOG(ERROR) << buf;
        break;
    }
    default:
        LOG(FATAL) << buf;
        break;
    }
}

void LoggerImpleGoogleGlog::init() {
    google::InitGoogleLogging(_module_name.c_str(), &PrefixAttacher);     
    // google::InitGoogleLogging(_module_name.c_str());

    // 初始化相关参数
    std::string log_dir = Logger::instance()->get_config_by_key("log_dir");
    if (log_dir.empty()) {
        log_dir = Logger::instance()->get_work_dir() + "/log"; 
    }
    std::string log_level = Logger::instance()->get_config_by_key("log_level");

    // 设置日志保存目录
    std::string log_dir_path = log_dir;
    FLAGS_log_dir = log_dir_path; 

    //设置特定严重级别的日志的输出目录和前缀。第一个参数为日志级别，第二个参数表示输出目录及日志文件名前缀
    std::string file_prefix = "log/" + _module_name;
    google::SetLogDestination(google::GLOG_INFO, file_prefix.c_str());
    google::SetLogDestination(google::GLOG_WARNING, file_prefix.c_str());
    google::SetLogDestination(google::GLOG_ERROR, file_prefix.c_str());
    google::SetLogDestination(google::GLOG_FATAL, file_prefix.c_str());

    //在日志文件名中级别后添加一个扩展名。适用于所有严重级别
    // google::SetLogFilenameExtension("logExtension");

    //设置最大日志文件大小（以MB为单位）
    FLAGS_max_log_size = 10;  

    //设置是否在磁盘已满时避免日志记录到磁盘
    FLAGS_stop_logging_if_full_disk = true;  

    //设置日志消息是否转到标准输出而不是日志文件
    // FLAGS_logtostderr = true;  
    //设置日志消息除了日志文件之外是否去标准输出(当前选择，终端和日志都输出，如果终端存在的话)
    FLAGS_alsologtostderr = true;  

    //大于指定级别的日志都输出到标准错误(包括自己)
    if (log_level.empty()) {
        google::SetStderrLogging(google::GLOG_WARNING);
    } else {
        if (log_level == "debug") {
            google::SetStderrLogging(google::GLOG_INFO);
        } else if (log_level == "info") {
            google::SetStderrLogging(google::GLOG_INFO);
        } else if (log_level == "warning") {
            google::SetStderrLogging(google::GLOG_WARNING);
        } else if (log_level == "fatal") {
            google::SetStderrLogging(google::GLOG_FATAL);
        }
    }
    
    //设置记录到标准输出的颜色消息（如果终端支持）
    //一般是从ERROR才有颜色，如果INFO有颜色可以添加：FLAGS_stderrthreshold=google::INFO;
    FLAGS_colorlogtostderr = true;  

    //设置日志前缀是否应该添加到每行输出
    // FLAGS_log_prefix = true;  

    //设置可以缓冲日志的最大秒数，0指实时输出
    FLAGS_logbufsecs = 0;
}

void LoggerImpleGoogleGlog::uinit() {
    // 必须关闭，否则有内存泄露
    google::ShutdownGoogleLogging();
}