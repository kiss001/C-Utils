#pragma once 

#include <vector>
#include <iostream>
#include <dirent.h>

#include "buffer.h"
#include "types.h"

// namespace scss {
typedef int (*filter)(const struct dirent*);
typedef int (*file_sort)(const struct dirent**, const struct dirent**);

class FileAuxiliary {
public:
    static bool read_file(const std::string& source, ByteArray& buf, size_t& size);
    static bool read_file(const std::string& source, Buffer& buf);
    static bool write_file(const Buffer& buf, const std::string& destinate);
    static bool write_file(const uint8_t* buffer, const size_t size,
            const std::string& destinate);
    static bool write_file(const uint8_t* buffer, const size_t size,
            const std::string& destinate, std::ios_base::openmode mode);

    static bool is_file_exist(const std::string& file);
    static bool lock_file(const std::string& file, int& fd);
    static void unlock_file(const int fd);
    static bool remove_file(const std::string& file);

    /*like bash command: mkdir -p*/
    static bool create_dir(const std::string& whole);
    static bool is_dir_exist(const std::string& file);
    static bool remove_dir(const std::string& dir);
    static bool remove_not_empty_dir(const std::string& dir, bool delete_root_dir = true);

    /*get names of all files in the folder and store into vector*/
    static bool list_files_under_dir(const std::string& dirpath, std::vector<std::string>& files);
    static bool list_files_sort_under_dir(const std::string& dirpath, filter file_filter,
             file_sort file_sort, std::vector<std::string>& files);

    static size_t files_total_size(const std::string& dirpath, const std::vector<std::string>& files);

private:
 //   FileAuxiliary();
};

// } /* namespace secure_keystore */

