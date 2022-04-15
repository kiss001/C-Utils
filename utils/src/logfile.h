#ifndef BASE_LOGGER_LOGFILE_H
#define BASE_LOGGER_LOGFILE_H

#include <stdint.h>
#include <string>

class LogFile {
public:
    static void construct_filepath(std::string& filepath, std::string& base_file);
    static bool createfile(FILE** file, std::string& file_path);
    static void writefile(FILE* file, const char* message, uint32_t message_len);

};

#endif