#include "ipc_auxiliary.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h> 
#include <assert.h>
#include <fcntl.h>

#include "logger.h"

// namespace scss {

IpcAuxiliary::IpcAuxiliary() {
    _s_server_stop = true;
    _s_msg_len_max = 1024 * 1024 * 2;
    _s_magic_number = 0x1bd509de;

    _server_sock = -1;

    memset(_request_handler, 0, sizeof(_request_handler));

    _ip = "127.0.0.1";
    _port = 6578;
}

IpcAuxiliary::~IpcAuxiliary() {
    
}

RtnCode IpcAuxiliary::service_start() {
    // 如果服务已经处理启动状态，不能重新启动。
    if (_s_server_stop == false) {
        loge("The service is running\n");

        return IPC_SERVICE_IS_RUNNING;
    }

    // 开始启动服务
    _s_server_stop = false;

    RtnCode rtn = RTN_OK;

    // 创建套接字
    _server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (0 > _server_sock) {
        loge("create AF_INET socket failed, errno is %d, errstr is %s\n", errno, strerror(errno));
        return IPC_SERVER_SOCK_INIT_ERR;
    }

    do {
        // 设置套接字信息
        int one = 1;
        setsockopt(_server_sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

        struct sockaddr_in server_addr;
        init_inet_sockaddr(reinterpret_cast<void*>(&server_addr));
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

        if (0 > bind(_server_sock, reinterpret_cast<const struct sockaddr*>(&server_addr),
                    sizeof(struct sockaddr_in))) {
            loge("bind server socket failed, errno is %d, errstr is %s\n", errno, strerror(errno));
            rtn = IPC_SERVER_SOCK_BIND_ERR;
            break;
        }

        // 监听套接字
        const uint32_t queue_len = 16;
        if (0 > listen(_server_sock, queue_len)) {
            loge("listen server socket failed, errno is %d, errstr is %s\n", errno, strerror(errno));
            rtn = IPC_SERVER_SOCK_LISTEN_ERR;
            break;
        }

        // 这种方式不会阻塞主线程
        // 处理信息
        if (0 != pthread_create(reinterpret_cast<pthread_t*>(&_tidp), NULL, 
                &IpcAuxiliary::svc_thread_func, (void*)this)) {
            loge("server loop err\n");
            rtn = IPC_SERVER_LOOP_ERR;
            break;
        }

        // 分离线程
        // pthread_detach(_tidp);

        // 二选一，这种方式会阻塞主线程
        // svc_thread_func_private();
    } while (0);

    if (RTN_OK != rtn) {
        close(_server_sock);
        _server_sock = -1;
    }
    
    return rtn;
}

void IpcAuxiliary::server_stop() {
    if (-1 != _server_sock) {
        close(_server_sock);
        _server_sock = -1;
    }

    // 通知分离线程退出
    // pthread_cancel(_tidp);

    _s_server_stop = true;

    return;
}

void IpcAuxiliary::init_inet_sockaddr(void* addr) {
    memset(addr, 0, sizeof(struct sockaddr_in));

    struct sockaddr_in* sockaddr = reinterpret_cast<struct sockaddr_in*>(addr);
    sockaddr->sin_family = AF_INET;

    sockaddr->sin_port = htons(_port);
    sockaddr->sin_addr.s_addr = inet_addr(_ip.c_str());

    return;
}

void* IpcAuxiliary::svc_thread_func(void* param) {
    IpcAuxiliary::instance()->svc_thread_func_private();

    return NULL;
}

void IpcAuxiliary::svc_thread_func_private() {
    fd_set sfds;
    struct timeval timeout;
    uint32_t maxfd = 0;
    bool rtn = true;

    signal(SIGPIPE, SIG_IGN);
    while(1) {
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
 
        FD_ZERO(&sfds);
        FD_SET(_server_sock, &sfds);
        maxfd = _server_sock;
        
        // 阻塞监听套接字,
        int ret = select(maxfd + 1, &sfds, NULL, NULL, &timeout);
        if (0 == ret) {
            if (_s_server_stop) {
                logi("ipc server stop\n");
                rtn = true;
                break;
            } 
            continue;
        } else if (0 > ret) {
            if (errno != EINTR) {
                loge("select server failed, errno is %d, errstr is %s\n", errno, strerror(errno));
                rtn = false;
            } else {
                rtn = true;
            }
            break;
        }

        if (FD_ISSET(_server_sock, &sfds)) {
            (void)svc_message_callback();
        } 
    }
}

bool IpcAuxiliary::svc_message_callback() {
    struct sockaddr_in client_addr;
    int client_sock = -1;
    socklen_t sock_len = 0;

    // 接受socket链接，然后对消息进行处理
    client_sock = accept(_server_sock, reinterpret_cast<struct sockaddr*>(&client_addr), &sock_len);
    if (0 > client_sock) {
        loge("accept client failed\n");
        return false;
    }

    bool rtn = true;
    do {
        ByteArray recv_msg;
        uint32_t recv_msg_len = 0;

        IpcProtocolHeader header = {0};

        // 接受数据
        if (!recv_message(client_sock, recv_msg, recv_msg_len, header)) {
            loge("recv message from client failed\n");
            rtn = false;
            break;
        }

        ByteArray rsp_msg;
        uint32_t rsp_msg_len = 0;

        // 回调处理
        RequestHandler msg_handler = _request_handler[header.moduleId];
        if (NULL == msg_handler) {
            rtn = false;
            break;
        }
        msg_handler(recv_msg.get(), recv_msg_len, rsp_msg, rsp_msg_len);

        // 发送消息
        if (!send_message(client_sock, rsp_msg.get(), rsp_msg_len, header)) {
            loge("send message to client failed\n");
            rtn = false;
            break;
        }
    } while (0);

    close(client_sock);
    client_sock = -1;

    return rtn;
}

void IpcAuxiliary::regist_request_handler(RpcSduType sdu_type, RequestHandler request_handle_cb) {
    assert(RST_MIN < sdu_type && sdu_type < RST_MAX);
    assert(_request_handler[sdu_type] == NULL);
    _request_handler[sdu_type] = request_handle_cb;
}

bool IpcAuxiliary::recv_message(int sock, ByteArray& recv_msg, uint32_t& recv_msg_len
    , IpcProtocolHeader& header) {
    if (!recv_buffer(sock, reinterpret_cast<uint8_t*>(&header), sizeof(header))) {
        loge("recv message header failed\n");
        return false;
    }

    if (_s_magic_number != header.magic) {
        loge("header's magic number unmatch, accept %x\n", header.magic);
        return false;
    }

    if (header.len > _s_msg_len_max) {
        loge("msg len %d overflow than allowed max len %d\n", header.len, _s_msg_len_max);
        return false;
    }

    recv_msg.reset(new uint8_t[header.len]);
    if (!recv_buffer(sock, recv_msg.get(), header.len)) {
        loge("recv message failed\n");
        return false;
    }

    recv_msg_len = header.len;
    return true;
}

bool IpcAuxiliary::send_message(int sock, const uint8_t* send_msg, uint32_t send_msg_len
    , IpcProtocolHeader& header) {
    if (!send_buffer(sock, reinterpret_cast<uint8_t*>(&header), sizeof(header))) {
        loge("send message header failed\n");
        return false;
    }

    if (!send_buffer(sock, send_msg, send_msg_len)) {
        loge("send message failed\n");
        return false;
    }

    return true;
}

bool IpcAuxiliary::recv_buffer(int sock, uint8_t* buf, uint32_t len) {
    uint32_t left_len = len;
    int received_len = 0;
    uint8_t* rbuf = buf;

    struct timeval timeout = {2, 0};
    if (0 != setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout))) {
        loge("set timeout for recv failed, errno is %d, errstr: %s\n", errno, strerror(errno));
        return false;
    }

    while (left_len > 0) {
        received_len = recv(sock, rbuf, left_len, 0);
        if (0 > received_len) {
            loge("recv from remote failed, expect len[%d], recv len[%d], errno is %d, errstr is %s\n", 
                    len, len - left_len, errno, strerror(errno));
            return false;
        } else if (0 == received_len) {
            loge("recv timeout\n");
            return false;
        }

        left_len -= received_len;
        rbuf += received_len;
    }

    return (left_len == 0);
}

bool IpcAuxiliary::send_buffer(int sock, const uint8_t* buf, uint32_t len) {
    uint32_t left_len = len;
    int32_t send_len = 0;
    const uint8_t* sbuf = buf;
    while (left_len > 0) {
        send_len = send(sock, sbuf, left_len, 0);
        if (0 > send_len) {
            loge("send to remote failed, expect len[%d], send len[%d], errno is %d, errstr is %s\n", 
                    len, len - left_len, errno, strerror(errno));
            break;
        }

        left_len -= send_len;
        sbuf += send_len;
    }

    return (left_len == 0);
}

RtnCode IpcAuxiliary::client_2_server(RpcSduType sdu_type, const uint8_t* req_msg, uint32_t req_msg_len, 
                    ByteArray& rsp_msg, uint32_t& rsp_msg_len) {
    struct sockaddr_in client_addr;
    init_inet_sockaddr(reinterpret_cast<void*>(&client_addr));

    int client_sock = -1;
    RtnCode rtn = init_client_socket(client_sock);
    if (RTN_OK != rtn) {
        loge("init client sock failed\n");
        return rtn;
    }

    do {
        IpcProtocolHeader header = {0};
        header.moduleId = sdu_type;
        header.len = req_msg_len;
        header.magic = _s_magic_number;

        if (!send_message(client_sock, req_msg, req_msg_len, header)) {
            loge("send message to server failed\n");
            rtn = IPC_CLIENT_SEND_ERR;
            break;
        }

        if (!recv_message(client_sock, rsp_msg, rsp_msg_len, header)) {
            loge("recv message from server failed\n");
            rtn = IPC_CLIENT_RECV_ERR;
            break;
        }
    } while (0);

    close(client_sock);
    return rtn;
}

RtnCode IpcAuxiliary::init_client_socket(int& client_sock) {
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (0 > client_sock) {
        loge("create client AF_INET socket failed, errno is %d, errstr is %s\n", errno, strerror(errno));
        return IPC_CLIENT_SOCK_INIT_ERR;
    }

    struct sockaddr_in server_addr;
    init_inet_sockaddr(&server_addr);

    if (0 > connect_nonblock(client_sock, &server_addr, sizeof(struct sockaddr_in), 1)) {
        loge("connect ton server failed\n");
        close(client_sock);
        return IPC_CONNECT_SERVER_ERR;
    }

    return RTN_OK;
}

int IpcAuxiliary::connect_nonblock(int server_sock, void* server_addr, int addr_len, int timeout) {
    struct timeval tval;

    /*set the socket as noblock mode*/
    // 非阻塞connect，因为阻塞的connect可能会等待一段时间。这段时间完全可以去做其他事情。
    int flags = fcntl(server_sock, F_GETFL, 0);
    fcntl(server_sock, F_SETFL, flags | O_NONBLOCK); //non-block socket

    int conn_state = -1;
    do {
        int connect_flag = connect(server_sock, reinterpret_cast<struct sockaddr*>(server_addr), addr_len);
        if ((0 > connect_flag) && (errno != EINPROGRESS)) {
            loge("nonblock connect ton server failed immdiately, errno is %d, errstr is %s\n", 
                    errno, strerror(errno));
            break;
        } else if (0 == connect_flag) {
            conn_state = 0;
            break;
        }

        tval.tv_sec = timeout;
        tval.tv_usec = 0;
 
        fd_set rest, west;
        FD_ZERO(&rest);
        FD_ZERO(&west);
        FD_SET(server_sock, &rest);
        FD_SET(server_sock, &west);
        int maxpd = server_sock + 1;
        int flag = select(maxpd, &rest, &west, NULL, timeout ? &tval : NULL);
        if (0 == flag) {
            loge("nonblockconnect failed, timeout\n");
            break;
        } else if (0 > flag) {
            loge("nonblock connect failed after try\n");
            break;
        }

        if (FD_ISSET(server_sock, &rest) && FD_ISSET(server_sock, &west)) {
            //readable and writealbe, need further judgement
            int error = 0;
            socklen_t len = sizeof(error);
            if (getsockopt(server_sock, SOL_SOCKET, SO_ERROR, &error, &len) < 0) {
                break;
            }

            loge("nonblock connect error = %d\n", error);
            if (error != 0) {
                //error is not 0, means connect failed
                loge("connect failed\n");
                break ;
            }

            conn_state = 0;
        } else if (FD_ISSET(server_sock, &west) && !FD_ISSET(server_sock, &rest)) {
            //writeable but not readable, means connect success
            //logi("connect success\n");
            conn_state = 0;
        }
    } while (0);

    fcntl(server_sock, F_SETFL, flags);
    return conn_state;
}

// }
