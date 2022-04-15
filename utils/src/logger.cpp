#include <errno.h>
#include <string.h>
#include <stdarg.h>

#include "logger.h"
#include "logfile.h"
#include "logmessage.h"

// #define SYSLOG

#if defined(GOOGLE_GLOG)
#include <glog/logging.h>
#elif defined(SYSLOG)
#include <syslog.h>
#endif

Logger::Logger() 
 : _module_name("LOG") {
     
#if defined(GOOGLE_GLOG)
    google::InitGoogleLogging(_module_name.c_str());
    FLAGS_logtostderr = 1;

    // google::InitGoogleLogging(module_name);
    // std::string logfile;
    // scss::ScssPathMgr::get_scss_log_dir_path(logfile);
//    FLAGS_log_dir = logfile.append("/glog/");

    // FLAGS_minloglevel =  google::INFO;
    // FLAGS_logtostderr = false;
    // FLAGS_alsologtostderr = false;
    // FLAGS_stop_logging_if_full_disk = true;
    // FLAGS_max_log_size = 1;
    // FLAGS_logbufsecs = 0;
    // logfile.append("/").append(module_name).append("_");
    // google::SetStderrLogging(google::GLOG_FATAL);
    // google::SetLogDestination(google::GLOG_FATAL, logfile.c_str());
    // google::SetLogDestination(google::GLOG_ERROR, logfile.c_str());
    // google::SetLogDestination(google::GLOG_WARNING, logfile.c_str());
    // google::SetLogDestination(google::GLOG_INFO, logfile.c_str());
#elif defined(SYSLOG)
    openlog(_module_name.c_str(), LOG_PID, LOG_LOCAL5);
#endif
}

Logger::~Logger() {
#if defined(GOOGLE_GLOG)
    google::ShutdownGoogleLogging();
#endif  
}

void Logger::log_print(LogLevel log_level, const char* format, ...) {
    va_list ap;
    va_start(ap, format);

    char buf[256];
    vsnprintf(buf, 256, format, ap);
    va_end(ap);

#if defined(GOOGLE_GLOG)
    LOG(INFO) << buf;
#elif defined(SYSLOG)
    syslog(log_level, "%s", buf);
#else
    printf("%s", buf);
#endif

// #elif defined(CLOG)
//     LOGGER(LogLevel(level), buf);

// #else
//     PLOG(level, buf);
    
// #endif
}