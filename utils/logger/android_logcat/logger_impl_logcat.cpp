#include "logger_impl_logcat.h"

// #include <utils/Log.h>
#include <iostream>

LoggerImpleLogcat::LoggerImpleLogcat() {

}

LoggerImpleLogcat::~LoggerImpleLogcat() {

}

void LoggerImpleLogcat::print(LogLevel log_level, const char* format, va_list args) {
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);

    switch (log_level)
    {
    case LOG_DEBUG: {
        break;
    }
    case LOG_INFO: {
        break;
    }
    case LOG_WARNING: {
        break;
    }
    case LOG_ERROR: {
        // ALOGE(format, args);
        break;
    }
    default:
        break;
    }
}