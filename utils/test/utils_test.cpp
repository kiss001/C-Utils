#include "string_utils.h"

#include <iostream>
#include <gtest/gtest.h>

#include "types.h"
#include "ipc_auxiliary.h"
#include "logger.h"

TEST(StringList, split) {
    ASSERT_TRUE(1);
}

RtnCode call_back(const uint8_t* recv_msg, uint32_t recv_msg_len, 
        ByteArray& rsp_msg, uint32_t& rsp_msg_len) {
    char send_msg[100] = "Hello world!!!";

    char* send_msg1 = new char[100];
    strncpy(send_msg1, send_msg, 100);
    
    rsp_msg.reset((uint8_t*)send_msg1);
    rsp_msg_len = 100;

    loge("%s.\n", send_msg);
}

TEST(IPC, ipc) {
    IpcAuxiliary::instance()->regist_request_handler(RST_ECHO, call_back);

    ASSERT_EQ(IpcAuxiliary::instance()->service_start(), RTN_OK);

    char send_msg[100] = "Hello world!!!";
    ByteArray rcv_msg;
    uint32_t rcv_msg_length = 0;

    IpcAuxiliary::instance()->client_2_server(RST_ECHO, (uint8_t*)send_msg, 100, rcv_msg, rcv_msg_length);

    ASSERT_STREQ((char*)rcv_msg.get(), send_msg);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}