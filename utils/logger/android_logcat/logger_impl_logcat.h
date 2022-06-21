#pragma once

#include "logger_impl.h"

class LoggerImpleLogcat : public LoggerImpl {
public:
    LoggerImpleLogcat();
    ~LoggerImpleLogcat();

    void print(LogLevel log_level, const char* format, va_list args);

private:


};