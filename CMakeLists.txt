# 设置CMake最小依赖
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# 设置工程名和语言
project(c++_utils LANGUAGES CXX)

cmake_policy(SET CMP0015 NEW)

############# 以下是环境检查和设置过程 #############
# 打印必要的系统信息
message(STATUS "system processor: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "system name: ${CMAKE_SYSTEM_NAME}")

# 打印必要的编译器信息
message(STATUS "COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "c++ flags, Debug Configuration: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "c++ flags, Release Configuration: ${CMAKE_CXX_FLAGS_RELEASE}")

# 打开编译过程中的详细信息
# set(CMAKE_VERBOSE_MAKEFILE on)

############# 以下是编译过程 #############
# 设置变量
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(THIRD_TOOLS_DIR ${ROOT_DIR}/third_tools)  # 第三方依赖库目录

# 增加子CMAKE_LIST.txt
add_subdirectory(utils)

add_subdirectory(socrule)               # 用于安全规则系统

add_subdirectory(device_info)           # 用于安全规则系统

############# 以下是测试过程 #############
# 启动单元测试
enable_testing()

# 增加测试用例
add_test(
    NAME utils_test
    COMMAND ${ROOT_DIR}/test/utils/utils_test
)

# 设置测试用例时间
set_tests_properties(utils_test PROPERTIES TIMEOUT 10)

