#ifndef BASE_LOGGER_LOGMESSAGE_H
#define BASE_LOGGER_LOGMESSAGE_H

#include <string>

class LogMessage {
public:
    static void construct_line_message(const char* level_name, std::string& message, std::string& line_message);
};

#endif
