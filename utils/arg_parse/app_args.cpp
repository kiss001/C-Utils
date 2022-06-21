#include "app_args.h"

#include "logger.h"
#include "string_utils.h"
#include "file_auxiliary.h"

#include <unistd.h>
#include <libgen.h>
#include <errno.h>

ApplicationArguments::ApplicationArguments() {
    // 初始化可执行文件所在的目录
    init_bin_dir_path();

    init_run_dir_path();
}

ApplicationArguments::~ApplicationArguments() {

}

RtnCode ApplicationArguments::init(int argc, const char* argv[]) {
    return parse(argc, argv);
}

void ApplicationArguments::uinit() {

}

StringVector ApplicationArguments::get_source_args() {
    return _source_args;
}

bool ApplicationArguments::contains_option(std::string name) {
    if (_option_args.find(name) != _option_args.end()) {
        return true;
    }

    return false;
}

std::string ApplicationArguments::get_option_values(std::string name) {
    if (_option_args.find(name) != _option_args.end()) {
        return _option_args[name];
    }

    return "";
}

StringVector ApplicationArguments::get_no_option_args() {
    return _non_option_args;
}

RtnCode ApplicationArguments::parse(int argc, const char* argv[]) {
    RtnCode rtn = RTN_OK;

    // 第一个参数为程序的名字
    for (int i = 1; i < argc; i++) {
        const char* arg = argv[i];

        std::string arg_str = arg;

        // 存储原始参数
        _source_args.push_back(arg_str);

        // 解析--a1=1234
        if (StringUtils::starts_with(arg_str, "--")) {
            std::string option_text = arg_str.substr(2);

            int index = option_text.find_first_of("=");
            if (index != -1) {
                std::string name = option_text.substr(0, index);
                std::string value = option_text.substr(index + 1);

                if (name.empty()) {
                    loge("The arguments %s parse failed.\n", arg_str.c_str());
                    assert(false);

                    rtn = ERROR_ARGS_PARSE_FILED;
                } else {
                    // 如果是路径转化为绝对路径
                    _option_args[name] = get_absolute_path(value);
                }
            } else {
                _option_args[option_text] = "";
            }

            continue;
        }

        // 解析-a -b
        if (StringUtils::starts_with(arg_str, "-")) {
            std::string option_text = arg_str.substr(1);

            if (option_text.empty()) {
                loge("The arguments %s parse failed.\n", arg_str.c_str());
                assert(false);

                rtn = ERROR_ARGS_PARSE_FILED;
            }

            _non_option_args.push_back(option_text);

            continue;
        }

        loge("The arguments %s parse failed.\n", arg_str.c_str());
        assert(false);

        rtn = ERROR_ARGS_PARSE_FILED;
    }

    return rtn;
}

RtnCode ApplicationArguments::init_run_dir_path() {
    char buff[1024] = {0};
    getcwd(buff, sizeof(buff));

    _run_dir_path = buff;

    return RTN_OK;
}

RtnCode ApplicationArguments::init_bin_dir_path() {
    char buf[1024];
    // int count = readlink("/proc/pid/exe", buf, 1024);
    int count = readlink("/proc/self/exe", buf, 1024);
    if (count < 0 || count >= 1024 ) {
        loge("readlink error: %s\n", strerror(errno));

        return ERROR_CONFIG_PARSE_FILED;
    }
    buf[count] = '\0';

    // 获取可执行文件所在目录的父目录
    _bin_dir_path = StringUtils::dir_name(buf);
    std::string temp = _bin_dir_path;
    _work_dir_path = StringUtils::dir_name(_bin_dir_path);

    // 程序名 basename
    // char* app = strrchr(buf, '/');
    // *app++ = '\0';

    return RTN_OK;
}

std::string ApplicationArguments::get_bin_dir_path() {
    return _bin_dir_path;
}

std::string ApplicationArguments::get_work_dir_path() {
    return _work_dir_path;
}

std::string ApplicationArguments::get_absolute_path(std::string relative_path, std::string current_dir) {
    if (!current_dir.empty()) {
        if (!FileAuxiliary::is_dir_exist(current_dir)) {
            loge("The current path is not directory \n", current_dir.c_str());
            assert(false);
        }
    }

    if (!StringUtils::starts_with(relative_path, "./") 
        && !StringUtils::starts_with(relative_path, "../")) {
        // logd("The path %s is not relative path\n", relative_path.c_str());
        return relative_path;
    }

    std::string prefix_str;
    if (current_dir.empty()) {
        prefix_str = _run_dir_path;
    } else {
        prefix_str = current_dir;
    }

    std::string suffix_str;

    if (StringUtils::starts_with(relative_path, "./")) {
       suffix_str = relative_path.substr(1);
    }

    if (StringUtils::starts_with(relative_path, "../")) {
        suffix_str = relative_path.substr(2);
        prefix_str = dirname((char*)(prefix_str.c_str()));
    }

    std::string path = prefix_str + suffix_str;

    logi("The relative path %s, absolute path is %s\n", relative_path.c_str(), path.c_str());
    return path;
}


