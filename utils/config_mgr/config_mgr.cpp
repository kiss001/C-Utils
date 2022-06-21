#include "config_mgr.h"

#include "file_auxiliary.h"
#include "rtn_code.h"
#include "logger.h"
#include "string_utils.h"
#include "app_args.h"

#include <assert.h>
#include <unistd.h>
#include <libgen.h>

ConfigMgr::ConfigMgr() {

}

ConfigMgr::~ConfigMgr() {
    (void)uinit();
}

RtnCode ConfigMgr::init(std::string config_path_name, ConfigFileType type) {
    RtnCode rtn = RTN_OK;
    
    // 第一，初始化
    _config_file_path = config_path_name;

    if (_config_file_path.empty()) {
        // 第二，解析配置文件
        std::string config_path = ApplicationArguments::instance()->get_option_values("conf_path");
        std::string config_type = ApplicationArguments::instance()->get_option_values("conf_type");

        // 未指定配置文件路径
        if (config_path.empty()) {
            // 第三，使用默认配置
            _config_file_path = ApplicationArguments::instance()->get_work_dir_path() + "/config/config.properties";
            type = E_CONFIG_FILE_TYPE_PROPTERTIES;
        } else {
            _config_file_path = config_path;

            // 未指定配置文件类型
            if (config_type.empty()) {
                type = E_CONFIG_FILE_TYPE_PROPTERTIES;
            } else {
                int type_num = atoi(config_type.c_str());
                if (type_num >= E_CONFIG_FILE_TYPE_MAX) {
                    loge("The config file type %d is incorrect.\n", type_num);

                    return ERROR_CONFIG_PARSE_FILED;
                }

                type = (ConfigFileType) type_num;
            }
        }
    }

    // 读取配置文件
    _config_dir_path = StringUtils::dir_name(_config_file_path);

    if (!FileAuxiliary::is_file_exist(_config_file_path)) {
        loge("The config file %s is not existed.\n", _config_file_path.c_str());
        return ERROR_CONFIG_PARSE_FILED;
    }

    ByteArray buffer;
    size_t size;
    if (!FileAuxiliary::read_file(_config_file_path, buffer, size)) {
        loge("The config file %s read failed.\n", _config_file_path.c_str());
        return ERROR_CONFIG_PARSE_FILED;
    }

    // 解析
    rtn = parse_config_file(buffer, size, type);
    
    return rtn;
}

void ConfigMgr::uinit() {

}

RtnCode ConfigMgr::parse_properties(ByteArray& buff, size_t size) {
    std::string configs((char*)buff.get(), size);
    StringVector vector;
    if (!StringUtils::split(configs, "\n", vector)) {
        return ERROR_CONFIG_PARSE_FILED;
    }

    for (int i = 0; i < vector.size(); ++i) {
        std::string line = vector.at(i);
        // 移除注释
        line = StringUtils::remove_comment(line);
        // 删除空格
        line = StringUtils::trim(line);
        if (line.empty()) {
            continue;
        }
        
        StringVector key_value;
        if (!StringUtils::split(line, "=", key_value) || (key_value.size() != 2)) {
            loge("The config record %s parse failed.\n", line.c_str());
            return ERROR_CONFIG_PARSE_FILED;
        }

        std::string key = StringUtils::trim(key_value[0]);
        std::string values = StringUtils::trim(key_value[1]);

        // 如果是相对路径转化为绝对路径, 绝对路径相对于配置文件所在目录
        values = ApplicationArguments::instance()->get_absolute_path(values, _config_dir_path);
        _config_infos[key] = values;
    }

    return RTN_OK;
}

RtnCode ConfigMgr::parse_config_file(ByteArray& buff, size_t size, ConfigFileType type) {
    RtnCode rtn = RTN_OK;

    // 解析各种类型的配置文件
    switch (type)
    {
    case E_CONFIG_FILE_TYPE_PROPTERTIES: {
        rtn = parse_properties(buff, size);
        break;
    }
    default:
        rtn = ERROR_CONFIG_PARSE_FILED;
        break;
    }

    return rtn;
}

std::string ConfigMgr::get_config_by_key(std::string key) {
    std::string command_arg = ApplicationArguments::instance()->get_option_values(key);
    if (!command_arg.empty()) {
        logi("The key \"%s\" from command arguments, value is %s.\n", key.c_str(), command_arg.c_str());

        return command_arg;
    }
    
    if (_config_infos.find(key) == _config_infos.end()) {
        // 这里不再判断配置是否存在，由使用配置的各个模块自己断言配置是否存在
        // assert(false);

        loge("The key \"%s\" is not exist.\n", key.c_str());
        return "";
    }

    logi("The key \"%s\" from config file, value is \"%s\".\n", key.c_str(), _config_infos[key].c_str());

    return _config_infos[key];
}

std::string ConfigMgr::get_data_dir() {
    std::string data_dir = get_config_by_key("data_dir");
    if (data_dir.empty()) {
        return ApplicationArguments::instance()->get_work_dir_path() + "/data";
    }

    return data_dir;
}


