#pragma once

#include <stdint.h>

// #ifdef __cplusplus
// namespace base {
// #endif

// ============== 基础定义 ==============
typedef uint32_t RtnCode;


// 错误码组成结构：
// 8bit（保留） | 8bit（模块名）| 16bit（错误码）
#define ERR_BIT 0x00000000
#define ERR_CODE(MODULE, CODE)              (0x00000000 | ((MODULE) << 16) | (CODE))

#define ERR_SUCCEED(rtncode) (((rtncode) & ERR_BIT) == 0)

// ============== 成功 ==============
//成功，没有任何异常
#define RTN_OK                              ((uint32_t)0x0)

// ============== 基础模块 ==============
#define BASE_MODULE                         ((uint32_t)0x1)

#define TIMER_ID_EXISTED                    ERR_CODE(BASE_MODULE, 0x01)
#define IPC_SERVER_SOCK_INIT_ERR            ERR_CODE(BASE_MODULE, 0x02)
#define IPC_SERVER_SOCK_BIND_ERR            ERR_CODE(BASE_MODULE, 0x03)
#define IPC_SERVER_SOCK_LISTEN_ERR          ERR_CODE(BASE_MODULE, 0x04)
#define IPC_SERVER_LOOP_ERR                 ERR_CODE(BASE_MODULE, 0x05)

#define IPC_CLIENT_SOCK_INIT_ERR            ERR_CODE(BASE_MODULE, 0x06)
#define IPC_CONNECT_SERVER_ERR              ERR_CODE(BASE_MODULE, 0x07)
#define IPC_CLIENT_RECV_ERR                 ERR_CODE(BASE_MODULE, 0x08)
#define IPC_CLIENT_SEND_ERR                 ERR_CODE(BASE_MODULE, 0x09)
#define IPC_SERVICE_IS_RUNNING              ERR_CODE(BASE_MODULE, 0x0A)

#define ERROR_ZIPFILE_OPEN_FILED            ERR_CODE(BASE_MODULE, 0x10)
#define ERROR_ZIPFILE_UNCOMPRESS_FILED      ERR_CODE(BASE_MODULE, 0x11)

#define ERROR_FILE_CREATE_FILED             ERR_CODE(BASE_MODULE, 0x20)

#define ERROR_CONFIG_PARSE_FILED            ERR_CODE(BASE_MODULE, 0x30)
#define ERROR_LOG_CONFIG_PARSE_FILED        ERR_CODE(BASE_MODULE, 0x31)

#define ERROR_ARGS_PARSE_FILED              ERR_CODE(BASE_MODULE, 0x40)

#define ERROR_CRYPTO_PARAM_ERROR            ERR_CODE(BASE_MODULE, 0x50)
#define ERROR_CRYPTO_INIT_ERROR             ERR_CODE(BASE_MODULE, 0x51)
#define ERROR_CRYPTO_ENCRYPT_ERROR          ERR_CODE(BASE_MODULE, 0x52)
#define ERROR_CRYPTO_DECRYPT_ERROR          ERR_CODE(BASE_MODULE, 0x53)
#define ERROR_DIGEST_INIT_ERROR             ERR_CODE(BASE_MODULE, 0x61)
#define ERROR_DIGEST_ERROR                  ERR_CODE(BASE_MODULE, 0x62)


// ============== 守护进程模块 ==============
#define SVC_MODULE                         ((uint32_t)0x2)

#define SERVICE_INIT_FAILED                 ERR_CODE(SVC_MODULE, 0x1)
#define SERVICE_START_FAILED                ERR_CODE(SVC_MODULE, 0x2)

// ============== 规则管理模块 ==============
#define RULE_MGR_MODULE                    ((uint32_t)0x3)

// 从云端获取规则失败
#define RULE_MGR_REQUEST_RULE_FAILED       ERR_CODE(RULE_MGR_MODULE, 0x1)
// 解析从云端的规则失败
#define RULE_MGR_RULE_PARSE_FAILED         ERR_CODE(RULE_MGR_MODULE, 0x2)
// 模块对应的规则不存在
#define RULE_MGR_MODULE_RULE_NOT_EXIST     ERR_CODE(RULE_MGR_MODULE, 0x3)
// 设备信息初始化失败
#define RULE_MGR_DEVICE_INFO_INIT_FAILED   ERR_CODE(RULE_MGR_MODULE, 0x4)

// ============== 规则应用模块 ==============
#define RULE_PROC_MODULE                   ((uint32_t)0x4)

// 反馈模块状态失败
#define RULE_PROC_REFRESH_STATE_FAILED     ERR_CODE(RULE_PROC_MODULE, 0x1)
// 安全模块获取安全规则信息失败
#define RULE_PROC_GET_RULE_FAILED          ERR_CODE(RULE_PROC_MODULE, 0x2)
// 安全模块报告规则应用状态失败
#define RULE_PROC_REPORT_RULE_FAILED       ERR_CODE(RULE_PROC_MODULE, 0x3)

// #ifdef __cplusplus
// }
// #endif

