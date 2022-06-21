#pragma once

#include "singleton.h"
#include "rtn_code.h"
#include "types.h"

#include <string>
#include <map>

enum ConfigFileType {
    E_CONFIG_FILE_TYPE_MIN = 0,
    E_CONFIG_FILE_TYPE_JSON,
    E_CONFIG_FILE_TYPE_PROPTERTIES,
    E_CONFIG_FILE_TYPE_MAX
};

class ConfigMgr : public Singleton<ConfigMgr> {
public:
    // 优先级顺序为：传入参数 > 命令行参数 --conf_path=xxxx --conf_type=xxx指定 > 默认路径,工作目录/config/config.properties
    // 配置模块初始化后，各个项的优先级顺序为：命令行指定 > 配置文件指定
    RtnCode init(std::string config_path_name = ""
        , ConfigFileType type = E_CONFIG_FILE_TYPE_PROPTERTIES);
    void uinit();

    // 根据配置的Key或者值，值的类型的格式由各个配置使用方保证，这里统一返回为字符串
    // 命令行参数配置的优先级高于配置文件的设置，配置文件配置高于默认配置
    // 配置可以指定相对路径，但是所有相对路径都会被转化为绝对路径，命令行相对路径为当前执行的目录，配置文件相对于配置文件所在的目录
    // 常用选项列表：data_dir: 数据目录
    // 配置区分大小写
    std::string get_config_by_key(std::string key);

    // 获取数据目录。数据默认为执行文件目录的父目录下的data文件夹，或者由配置文件配置项 data_dir 指定
    std::string get_data_dir();

private:
    friend class Singleton<ConfigMgr>;
    ConfigMgr();
    ~ConfigMgr();

    // 配置文件所在的路径
    std::string _config_file_path;
    std::string _config_dir_path;

    // 配置项，以Key:Value的方式保存，Value统一存储为string，实际使用的时候做类型转换
    std::map<std::string, std::string> _config_infos;
    
    RtnCode parse_config_file(ByteArray& buff, size_t size, ConfigFileType type);
    RtnCode parse_properties(ByteArray& buff, size_t size);

};