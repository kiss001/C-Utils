#include <time.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <sstream>

#include "logfile.h"

void LogFile::construct_filepath(std::string &filepath, std::string& base_file) {
    time_t t = time(0);
    struct tm* tm_time = localtime(&t);

    std::ostringstream time_pid_stream;
    time_pid_stream.fill('0');
    time_pid_stream << 1900 + tm_time->tm_year
                    << std::setw(2) << 1 + tm_time->tm_mon
                    << std::setw(2) << tm_time->tm_mday
                    << '-'
                    << std::setw(2) << tm_time->tm_hour
                    << std::setw(2) << tm_time->tm_min
                    << std::setw(2) << tm_time->tm_sec
                    << '.'
                    << getpid();
    
    filepath = base_file;
    filepath += time_pid_stream.str();
}

bool LogFile::createfile(FILE** file, std::string& file_path) {
    assert(file_path.size() != 0);

    int fd = open(file_path.c_str(), O_WRONLY | O_CREAT, 0664);
    if (fd == -1) {
        return false;
    }

    *file = fdopen(fd, "a");
    if (*file == NULL) {
        close(fd);
        return false;
    }

    return true;
}

void LogFile::writefile(FILE* file, const char* message, uint32_t message_len) {
    assert(file != NULL);

    fwrite(message, 1, message_len, file);
    fflush(file);
}
