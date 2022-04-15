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

#define TIMER_ID_EXISTED                    ERR_CODE(BASE_MODULE, 0x1)
#define IPC_SERVER_SOCK_INIT_ERR            ERR_CODE(BASE_MODULE, 0x2)
#define IPC_SERVER_SOCK_BIND_ERR            ERR_CODE(BASE_MODULE, 0x3)
#define IPC_SERVER_SOCK_LISTEN_ERR          ERR_CODE(BASE_MODULE, 0x4)
#define IPC_SERVER_LOOP_ERR                 ERR_CODE(BASE_MODULE, 0x5)

#define IPC_CLIENT_SOCK_INIT_ERR            ERR_CODE(BASE_MODULE, 0x6)
#define IPC_CONNECT_SERVER_ERR              ERR_CODE(BASE_MODULE, 0x7)
#define IPC_CLIENT_RECV_ERR                 ERR_CODE(BASE_MODULE, 0x8)
#define IPC_CLIENT_SEND_ERR                 ERR_CODE(BASE_MODULE, 0x9)
#define IPC_SERVICE_IS_RUNNING              ERR_CODE(BASE_MODULE, 0xA)


// #ifdef __cplusplus
// }
// #endif

