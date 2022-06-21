#pragma once

#include <string>
#include <vector>

#include "rtn_code.h"
#include "unzip.h"
#include "stdint.h"

typedef struct ZipFileInfo {
    std::string filename;
    // 文件内容经过Base64编码
    std::string content;
} S_ZIP_FILE_INFO;

class ZipAction {
public:
    // 本函数解压文件内存，适用于小文件的情况。这种情况过滤掉了压缩包文件列表中的目录
    static RtnCode uncompress_zip(const std::string& filename, std::vector<S_ZIP_FILE_INFO>& files);
    static RtnCode do_extract(const unzFile handler, const std::string& password, std::vector<S_ZIP_FILE_INFO>& files); 
    static RtnCode do_extract_currentfile(const unzFile handler, const std::string& password, std::vector<S_ZIP_FILE_INFO>& files);

private:

};