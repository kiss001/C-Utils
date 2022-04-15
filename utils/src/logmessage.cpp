#include <stdio.h>
#include <time.h>

#include "logmessage.h"

void LogMessage::construct_line_message(const char* level_name,
            std::string& message, std::string& line_message) {
    time_t t;
    time(&t);
    std::string stime = ctime(&t);

    line_message = stime.substr(0, 20);
    line_message.append(level_name).append(" ").append(message);
}