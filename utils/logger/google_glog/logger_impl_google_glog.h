#pragma once

#include "logger_impl.h"

#include <string>

class LoggerImpleGoogleGlog : public LoggerImpl {
public:
    LoggerImpleGoogleGlog(std::string module_name);
    ~LoggerImpleGoogleGlog();

    void print(LogLevel log_level, const char* format, va_list args);

private:
    std::string _module_name;

    void init();
    void uinit();

};