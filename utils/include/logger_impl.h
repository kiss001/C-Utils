#pragma once

#include <stdarg.h>

enum LogLevel {
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
};

class LoggerImpl {
public:
    LoggerImpl() {}
    virtual ~LoggerImpl() {}

    virtual void print(LogLevel log_level, const char* format, va_list args) {};

private:

};