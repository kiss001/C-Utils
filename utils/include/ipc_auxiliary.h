/**
 * @file ipc_auxiliary.h
 * @author your name (you@domain.com)
 * @brief  IPC通信的架构为：
 * 1 服务端： 主线程创建服务，服务启动线程监听连接套接字，执行回调函数
 * 2 客户端： 
 * @version 0.1
 * @date 2022-03-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

// #include "scss_typedef.h"
// #include "memory_auxiliary.h"
#include "types.h"
#include "rtn_code.h"
#include "singleton.h"

// namespace scss {

// 定义各个消息模块的模块ID
typedef enum {
    RST_MIN = 0,
    RST_ECHO,
    RST_HEARTBEAT,
    RST_CM,
    RST_CRL,
    RST_OM,
    RST_TRMS_LOCATION,
    RST_TRMS_CARID,
    RST_MAX,
} RpcSduType;

typedef RtnCode (*MsgHandler)(const uint8_t* recv_msg, uint32_t recv_msg_len, 
        ByteArray& rsp_msg, uint32_t& rsp_msg_len);
typedef MsgHandler RequestHandler;

class IpcAuxiliary : public Singleton<IpcAuxiliary> {
public:
    // 服务启动和停止，服务启动和停止，应该主线程中完成。
    RtnCode service_start();
    void server_stop();

    static void* svc_thread_func(void* param);
    void svc_thread_func_private();
    bool svc_message_callback();

    // 注册各个模块的回调函数。
    void regist_request_handler(RpcSduType sdu_type, RequestHandler request_handle_cb);

    // 客户端发送消息，调用者应该是在一个独立的线程中完成。
    RtnCode client_2_server(RpcSduType sdu_type, const uint8_t* req_msg, uint32_t req_msg_len, 
                    ByteArray& rsp_msg, uint32_t& rsp_msg_len);

private:
    friend class Singleton<IpcAuxiliary>;
    IpcAuxiliary();
    ~IpcAuxiliary();

    typedef struct {
        uint8_t version;
        uint8_t moduleId;
        uint32_t magic;
        uint32_t len;
    } IpcProtocolHeader;

    uint32_t _s_msg_len_max;
    uint32_t _s_magic_number;

    // 服务状态
    bool _s_server_stop;

    int _server_sock;
    pthread_t _tidp;

    RequestHandler _request_handler[RST_MAX]; 

    // 服务监听的IP和端口号
    std::string _ip;
    short _port;

private:
    void init_inet_sockaddr(void* addr);
    
    bool recv_message(int sock, ByteArray& recv_msg, uint32_t& recv_msg_len
        , IpcProtocolHeader& header);
    bool send_message(int sock, const uint8_t* send_msg, uint32_t send_msg_len, IpcProtocolHeader& header);

    bool recv_buffer(int sock, uint8_t* buf, uint32_t len);
    bool send_buffer(int sock, const uint8_t* buf, uint32_t len);

    RtnCode init_client_socket(int& client_sock);
    int connect_nonblock(int server_sock, void* server_addr, int addr_len, int timeout);

};

// }

