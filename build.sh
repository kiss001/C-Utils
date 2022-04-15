#!/bin/bash

# 初始化配置变量，这一步可以设计由其他脚本设置。方式是configure.sh写入配置到env.sh中，然后执行source env.sh
BUILD_TOOL_CHAIN_FILE="../toolchain.cmake"
BUILD_TYPE="Debug"

# 创建编译文件相关文件夹
rm -rf lib
rm -rf test
rm -rf build
if [ ! -d "./build" ]; then
    mkdir build
fi
cd build

# 在父目录寻找CMAKE文件，在当前目录生成相关文件，CMAKE_TOOLCHAIN_FILE指定编译工具链
cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=${BUILD_TOOL_CHAIN_FILE} \
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \

# 执行makefile
make
