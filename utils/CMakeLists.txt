# 设置项目名称
project(UTILS)

############# 设置一些变量 ############# 
set(UTILS_DIR ${ROOT_DIR}/utils)
set(UTILS_DIR_TEST ${UTILS_DIR}/test)

############# 编译 ############# 
# 递归查找源文件
file(GLOB_RECURSE src_files 
    ${UTILS_DIR}/arg_parse/*.cpp
    ${UTILS_DIR}/basic_types/*.cpp
    ${UTILS_DIR}/config_mgr/*.cpp
    ${UTILS_DIR}/file_auxiliary/*.cpp
    ${UTILS_DIR}/file_auxiliary/*.c
    ${UTILS_DIR}/logger/*.cpp
    ${UTILS_DIR}/memory_auxiliary/*.cpp
    ${UTILS_DIR}/random/*.cpp
    ${UTILS_DIR}/other/*.cpp
    ${UTILS_DIR}/process_auxiliary/*.cpp
    ${UTILS_DIR}/thread_pool/*.cpp
    ${UTILS_DIR}/timer_mgr/*.cpp
    ${UTILS_DIR}/crypto/*.cpp
    ${THIRD_TOOLS_DIR}/zip/include/*.c
)

# 设置链接库输出目录
set(LIBRARY_OUTPUT_PATH ${ROOT_DIR}/lib)

# 设置目标
ADD_LIBRARY(utils ${src_files})
ADD_LIBRARY(utils_so SHARED ${src_files})

# 设置头文件查找目录
# target_include_directories(utils PUBLIC ${UTILS_DIR}/include)
include_directories(
    ${UTILS_DIR_SRC}
    ${UTILS_DIR}/include
    ${UTILS_DIR}/logger/google_glog
    ${UTILS_DIR}/crypto/include
    ${UTILS_DIR}/crypto/crypto_auxiliary/crypto_openssl
    ${THIRD_TOOLS_DIR}/zip/include
    ${THIRD_TOOLS_DIR}/glog/include
)

# 设置链接库搜索目录
link_directories(
    ${ROOT_DIR}/lib
    ${THIRD_TOOLS_DIR}/zip/lib
    ${THIRD_TOOLS_DIR}/glog/lib
    ${THIRD_TOOLS_DIR}/openssl/lib
)

# 设置依赖的链接库
# target_link_libraries(a PRIVATE b)

# 设置编译选项

# 设置目标文件属性
set_target_properties(utils_so PROPERTIES OUTPUT_NAME utils)
set_target_properties(utils PROPERTIES CLEAN_DIRECT_OUTPUT 1)

############# 测试用例 ############# 
# 启动测试用例
file(GLOB_RECURSE test_src_files 
    ${UTILS_DIR_TEST}/*.cpp
)
# set(test_src_files ${UTILS_DIR_TEST}/*.cpp)
set(EXECUTABLE_OUTPUT_PATH ${ROOT_DIR}/test/utils)
add_executable(utils_test ${test_src_files})
target_link_libraries(utils_test PRIVATE 
    utils 
    gtest 
    glog 
    ssl 
    crypto 
    pthread
)