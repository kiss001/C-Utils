/**
 * @file app_args.h
 * @author your name (you@domain.com)
 * @brief 
 * 关键命令行参数：
 * --log_conf  : 日志模块配置文件路径。默认值：工作目录config/log.conf
 * --conf_path : 配置管理模块配置文件路径，配套还是有--conf_type。默认值：工作目录 config/config.properties
 * --data_dir  : 数据存放的目录。默认值：工作目录，data文件夹
 * @version 0.1
 * @date 2022-06-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include "singleton.h"
#include "rtn_code.h"
#include "types.h"

#include <string>
#include <map>

class ApplicationArguments : public Singleton<ApplicationArguments> {
public:
    RtnCode init(int argc, const char* argv[]);
    void uinit();

    // 获取所有的参数
    StringVector get_source_args();

    // 选项参数操作。所有类似--conf=xxxx --version都通过这种方式获取
    bool contains_option(std::string name);
    std::string get_option_values(std::string name);

    // 非选项操作操作。所有类似-a  -b  -c都通过这个函数获取
    StringVector get_no_option_args();

    // 获取可执行文件所在的目录
    std::string get_bin_dir_path();

    // 获取工作目录，为bin文件所在目录的父目录
    std::string get_work_dir_path();

    // 相对路径转化为绝对路径，相对于执行命令的目录，或者自己指定目录
    std::string get_absolute_path(std::string relative_path, std::string current_dir = "") ;

private:
    friend Singleton<ApplicationArguments>;
    ApplicationArguments();
    ~ApplicationArguments();

    // 原始参数
    StringVector _source_args;

    // 非选项参数
    StringVector _non_option_args;

    // 选项参数
    std::map<std::string, std::string> _option_args;

    // 可执行程序所在的目录
    std::string _bin_dir_path;

    // 工作目录，可执行文件目录的父目录
    std::string _work_dir_path;

    // 运行程序所在的目录
    std::string _run_dir_path;

    RtnCode init_bin_dir_path();
    RtnCode init_run_dir_path();

    // 参数解析，当前支持的参数类型：
    // -d -e -f
    // --version=12314
    // --version
    RtnCode parse(int argc, const char* argv[]);

};