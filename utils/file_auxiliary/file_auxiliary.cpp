/*
 * file_assistance.cpp
 *
 *  Created on: Oct 11, 2017
 *      Author: paddle
 */
#include "file_auxiliary.h"

#include <unistd.h>
#include <dirent.h>
#include <netinet/in.h>
#include <net/if.h>  
#include <sys/ioctl.h>  
#include <sys/stat.h>
#include <sys/types.h>  
#include <sys/file.h>
#include <sched.h>
#include <errno.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <fcntl.h>

#include <assert.h>

#include "auxiliary.h"
#include "logger.h"

// namespace scss {

bool FileAuxiliary::read_file(const std::string& source, ByteArray& buf, size_t& size) {
    assert(!source.empty());

    /*ios::ate - location the end of the file to get file size by tellg()*/
    std::ifstream in(source.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

    if (!in.is_open()) {
        // loge("cann\'t open the file, file path is %s\n", source.c_str());
        return false;
    }

    size = in.tellg();
    in.seekg(0, std::ios::beg);

    buf.reset(new uint8_t[size]);
    if (!in.read(reinterpret_cast<char*>(buf.get()), size)) {
        // loge("read file %s failed, expect read size[%zu], get[%ld]\n", source.c_str(), size, in.gcount());
        in.close();
        return false;
    }
    
    in.close();
    return true;
}

bool FileAuxiliary::read_file(const std::string& source, Buffer& buf) {
    assert(!source.empty());

    /*ios::ate - location the end of the file to get file size by tellg()*/
    std::ifstream in(source.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

    if (!in.is_open()) {
        loge("cann\'t open the file, file path is %s\n", source.c_str());
        return false;
    }

    uint32_t size = in.tellg();
    in.seekg(0, std::ios::beg);

    buf.alloc(size);
    if (!in.read(reinterpret_cast<char*>(buf.buf()), size)) {
        loge("read file %s failed, expect read size[%zu], get[%ld]\n", source.c_str(), size, in.gcount());
        in.close();
        return false;
    }
    
    in.close();
    return true;
}

bool FileAuxiliary::write_file(const Buffer& buf, const std::string& destinate) {
    return write_file(buf.buf(), buf.len(), destinate, std::ios::out | std::ios::binary | std::ios::ate);
}

bool FileAuxiliary::write_file(const uint8_t* buffer, const size_t size, const std::string& destinate) {
    return write_file(buffer, size, destinate, std::ios::out | std::ios::binary | std::ios::ate);
}

bool FileAuxiliary::write_file(const uint8_t* buffer, const size_t size,
                const std::string& destinate, std::ios_base::openmode mode) {
    if (buffer == NULL || size == 0 || destinate.empty()) {
        return false;
    }

    std::string catalog = destinate.substr(0, destinate.find_last_of("/"));

    if (!create_dir(catalog)) {
        loge("write_file failed, create dir error, path: %s, error: %s\n", 
            destinate.c_str(), strerror(errno));
        return false;
    }
    
    std::ofstream out(destinate.c_str(), mode);
            //std::ios::out | std::ios::binary | std::ios::ate);

    if (!out.is_open()) {
        loge("write_file failed, open file, error is: %s\n", strerror(errno));
        return false;
    }

    out.write(reinterpret_cast<const char*>(buffer), size);
    out.close();
    return true;
}

bool FileAuxiliary::is_file_exist(const std::string& file) {
    assert(!file.empty());

    struct stat statbuf;
    return (!stat(file.c_str(), &statbuf) && (0 != (S_IFREG & statbuf.st_mode)));
}

bool FileAuxiliary::lock_file(const std::string& file, int& fd) {
    int fd_tmp = open(file.c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (0 > fd_tmp) {
        return false;
    }

    if (0 > flock(fd_tmp, LOCK_EX | LOCK_NB)) {
        close(fd_tmp);
        return false;
    }
    fd = fd_tmp;
    return true;
}

void FileAuxiliary::unlock_file(const int fd) {
    flock(fd, LOCK_UN | LOCK_NB);
    close(fd);
}


bool FileAuxiliary::remove_file(const std::string& file) {
    assert(!file.empty());

    struct stat statbuf;
    if (!stat(file.c_str(), &statbuf) && (0 != (S_IFREG & statbuf.st_mode))) {
        if (remove(file.c_str()) < 0) {
            loge("remove_file %s failed, errno is %d, errstr is %s\n", 
                    file.c_str(), errno, strerror(errno));
            return false;
        }
    }
    return true;
}

bool FileAuxiliary::create_dir(const std::string& whole) {
    assert(!whole.empty());

    std::vector<std::string> paths;
    if (!Auxiliary::split_string_by_substr(whole, "/", paths)) {
        loge("error happened when parse line in create_dir\n");
        return false;
    }

    /*distinguish the absolute path or relative path*/
    std::string cur_path = "";

    if (whole.find("/") != std::string::npos && whole.find("/") == 0) {
        cur_path = '/';
    }

    for (std::vector<std::string>::iterator iter = paths.begin();
            iter != paths.end(); ++iter) {
        cur_path = cur_path + *iter;
        struct stat statbuf;

        if (stat(cur_path.c_str(), &statbuf)
                || 0 == (S_IFDIR & statbuf.st_mode)) {
            if (mkdir(cur_path.c_str(), 0700) < 0) {
                loge("mkdir %s failed\n", cur_path.c_str());
                return false;
            }
        }

        cur_path += '/';
    }

    return true;
}

bool FileAuxiliary::is_dir_exist(const std::string& dir) {
    assert(!dir.empty());

    struct stat statbuf;
    return (!stat(dir.c_str(), &statbuf) && (0 != (S_IFDIR & statbuf.st_mode)));
}

bool FileAuxiliary::remove_dir(const std::string& dir) {
    assert(!dir.empty());

    struct stat statbuf;
    if (!stat(dir.c_str(), &statbuf) && (0 != (S_IFDIR & statbuf.st_mode))) {
        if (rmdir(dir.c_str()) < 0) {
            loge("remove dir %s failed\n", dir.c_str());
            //loge("remove dir %s failed, errno %d, errstr %s\n", dir.c_str(), errno, strerror(errno));
            return false;
        }
    }
    return true;
}

bool FileAuxiliary::remove_not_empty_dir(const std::string& dir, bool delete_root_dir) {
    char cur_dir[] = ".";
    char up_dir[] = "..";
    std::string dir_name;
    // char dir_name[256] = {0};
    DIR* dirp = NULL;
    struct dirent* dp = NULL;
    struct stat dir_stat;

    // 参数传递进来的目录不存在，直接返回
    if (0 != access(dir.c_str(), F_OK)) {
        return false;
    }

    // 获取目录属性失败，返回错误
    if (0 > stat(dir.c_str(), &dir_stat)) {
        loge("get directory stat error");
        return false;
    }

    // 普通文件直接删除
    if (S_ISREG(dir_stat.st_mode)) {  
        remove(dir.c_str());
    } else if (S_ISDIR(dir_stat.st_mode)) {
        // 目录文件，递归删除目录中内容
        dirp = opendir(dir.c_str());
        while ((dp = readdir(dirp)) != NULL) {
            // 忽略 . 和 ..
            if ((0 == strcmp(cur_dir, dp->d_name)) || (0 == strcmp(up_dir, dp->d_name))) {
                continue;
            }
            
            dir_name = dir + "/" + dp->d_name;
            // snprintf(dir_name, sizeof(dir_name), "%s/%s", dir.c_str(), dp->d_name);

            // 递归调用
            remove_not_empty_dir(dir_name);
        }
        closedir(dirp);

        // 删除空目录
        if (delete_root_dir) {
            if (rmdir(dir.c_str()) < 0) {
                loge("remove dir %s failed\n", dir.c_str());
                return false;
            }   
        }
    } else {
        loge("unknow file type!");  
    }
    
    return true;
}

bool FileAuxiliary::list_files_under_dir(const std::string& dirpath, std::vector<std::string>& files) {
    assert(!dirpath.empty());
 
    struct stat s = {0};
    lstat(dirpath.c_str(), &s);
    if (!S_ISDIR(s.st_mode)) {
        // loge("list_files_under_dir failed, %s is not a dir\n", dirpath.c_str());
        return false;  /*not a dirctory*/
    }
    
    struct dirent* filename;    // return value for readdir()
    DIR * dir = opendir(dirpath.c_str());
    if (NULL == dir) {
        logi("list_files_under_dir failed, open dir %s failed\n", dirpath.c_str());
        return false;
    }

    while ((filename = readdir(dir)) != NULL ) {
        // get rid of "." and ".."
        if (strcmp(filename->d_name, ".") == 0 || 
            strcmp(filename->d_name, "..") == 0) {
            continue;
        }

        files.push_back(filename->d_name);
    }

    closedir(dir);
    return true;
}

bool FileAuxiliary::list_files_sort_under_dir(const std::string& dirpath, filter file_filter,
        file_sort file_sort, std::vector<std::string>& files) {
    assert(!dirpath.empty());

    struct stat s = {0};
    lstat(dirpath.c_str(), &s);
    if (!S_ISDIR(s.st_mode)) {
        loge("list_files_sort_under_dir failed, %s is not a dir\n", dirpath.c_str());
        return false;  /*not a dirctory*/
    }

    struct dirent** namelist;
    int filecount = scandir(dirpath.c_str(), &namelist, file_filter, file_sort);
    if (filecount < 0) {
        loge("list_files_sort_under_dir failed, scandir %s failed\n", dirpath.c_str());
        return false;
    }
    for (int i = 0; i < filecount; ++i) {
        files.push_back(namelist[i]->d_name);
        free(namelist[i]);
    }

    free(namelist);
    return true;
}

size_t FileAuxiliary::files_total_size(const std::string& dirpath, const std::vector<std::string>& files) {
    size_t size = 0;
    std::string filename;
    struct stat statbuf = {0};
    for (size_t i = 0; i < files.size(); ++i) {
        filename = dirpath;
        filename += '/';
        filename += files[i];
        if (stat(filename.c_str(), &statbuf) == -1) {
            loge("files_total_size stat %s failed, %s\n", filename.c_str(), strerror(errno));
            continue;
        }
        size += statbuf.st_size;
    }
    
    return size;
}

// } /* namespace scss */
