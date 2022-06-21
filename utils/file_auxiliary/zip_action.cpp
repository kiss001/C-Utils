#include "zip_action.h"

#include "logger.h"
#include "types.h"
#include "base64.h"

// #include <assert.h>

#define BUFFER_SIZE 4096

/**
 * @brief 解压缩文件到指定目录
 * 
 * @param filename 
 * @param dir 
 * @return RtnCode 
 */
RtnCode ZipAction::uncompress_zip(const std::string& filename, std::vector<S_ZIP_FILE_INFO>& files) {
    unzFile handler = unzOpen64(filename.c_str());
    if (handler == NULL) {
        loge("The Operation of Open zipFile is Failed, zip = %s\n", filename.c_str());
        return ERROR_ZIPFILE_OPEN_FILED;
    }
    
    RtnCode ret = RTN_OK;
    ret = do_extract(handler, "", files);
    if (RTN_OK != ret) {
        ret = ERROR_ZIPFILE_UNCOMPRESS_FILED;
    }

    unzClose(handler);
    return ret;
}

RtnCode ZipAction::do_extract(const unzFile handler, const std::string& password, std::vector<S_ZIP_FILE_INFO>& files) {
    RtnCode ret = RTN_OK;
    unz_global_info64 gi;

    ret = unzGetGlobalInfo64(handler, &gi);
    if (ret != UNZ_OK) {
        loge("The Operation of zipfile in unzGetGlobalInfo is Failed, ret = 0x%08x\n", ret);
        return ERROR_ZIPFILE_UNCOMPRESS_FILED;
    }

    // 解压缩每个文件
    for (size_t i = 0; i < gi.number_entry; ++i) {
        ret = do_extract_currentfile(handler, password, files);
        if (ret != UNZ_OK) {
            loge("The Operation of do_extract_currentfile is Failed, ret = 08%08x\n", ret);
            return ERROR_ZIPFILE_UNCOMPRESS_FILED;
        }

        if ((i + 1) < gi.number_entry) {
            ret = unzGoToNextFile(handler);
            if (ret != UNZ_OK) {
                loge("The Operation of zipfile in unzGoToNextFile, ret = 0x%08x\n", ret);
                return ERROR_ZIPFILE_UNCOMPRESS_FILED;
            }
        }
    }
    return RTN_OK;
}

RtnCode ZipAction::do_extract_currentfile(const unzFile handler, const std::string& password, std::vector<S_ZIP_FILE_INFO>& files) {
    RtnCode ret = RTN_OK;

    // 第一，需要判断当前的是否是文件或者文件夹。这里仅仅只有文件
    char filename_inzip[256];
    unz_file_info64 file_info;
    ret = unzGetCurrentFileInfo64(handler, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);
    if (ret != UNZ_OK) {
        printf("The Operation of unzGetCurrentFileInfo is Failed, ret = 0x%08x\n", ret);
        return ERROR_ZIPFILE_UNCOMPRESS_FILED;
    }

    // 第二，抽取的当前文件为文件夹，创建完成后直接返回成功
    // std::string item_str(dir + "/" + filename_inzip);
    std::string item_str(filename_inzip);
    if (item_str.find_last_of("/") == (item_str.length() - 1)) {
        // if (!FileAuxiliary::create_dir(item_str)) {
        //     loge("The Operation of Create_dir is Failed\n");
        //     return SCMF_ERROR_DIR_EXIST_CREATE;
        // } else {
        //     return RTN_OK;
        // }
        return RTN_OK;
    }

    // 如果是文件，则打开和读取文件
    ret = unzOpenCurrentFilePassword(handler, NULL);
    if (ret != UNZ_OK) {
        loge("The Operation of unzOpenCurrentFilePassword is Failed, ret = 0x%08x\n", ret);
        return ret;
    }

    // /*打开或者创建待抽取的文件，当目录不存在时，创建对应的目录，并重新打开和创建文件*/
    // FILE *fout = fopen(item_str.c_str(), "wb");
    // if (fout == NULL && item_str.find_last_of("/") != std::string::npos) {
    //     if (!FileAuxiliary::create_dir(item_str.substr(0, item_str.find_last_of("/")))) {
    //         loge("The Operation of Create_dir is Failed\n");
    //         ret = unzCloseCurrentFile(handler);
    //         return ret;
    //     }
    //     fout = fopen(item_str.c_str(), "wb");
    // }

    // if (fout == NULL) {
    //     ret = unzCloseCurrentFile(handler);
    //     loge("The Operation of OpenCreate File after Dir Exist is Failed, ret = 0x%08x\n", ret);
    //     return ret;
    // }


    std::string file_content;

    ByteArray buffer;
    buffer.reset(new uint8_t[BUFFER_SIZE]);

    do {
        ret = unzReadCurrentFile(handler, buffer.get(), BUFFER_SIZE);
        if (ret < 0) {
            loge("error %d with zipfile in unzReadCurrentFile\n", ret);
            break;
        }

        if (ret > 0) {
            file_content += base64_encode(std::string((char*)buffer.get(), ret));
        }

        // if (ret > 0) {
        //     if (fwrite(blob.buf.get(), ret, sizeof(uint8_t), fout) != sizeof(uint8_t)) {
        //         loge("The Operation of Write File is Failed\n");
        //         break;
        //     }
        // }
    } while (ret > 0);

    S_ZIP_FILE_INFO file_infos;
    file_infos.filename = std::string(filename_inzip);
    file_infos.content = file_content;

    files.push_back(file_infos);

    // 关闭文件
    // fclose(fout);
    ret |= unzCloseCurrentFile(handler);

    if (ret != UNZ_OK) {
        loge("The Operation of unzCloseCurrentFile is Failed, ret = 0x%08x\n", ret);
    }

    return ret;
}
