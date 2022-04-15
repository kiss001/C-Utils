# 设置项目名称
project(UTILS)

############# 设置一些变量 ############# 
set(UTILS_DIR ${ROOT_DIR}/utils)
set(UTILS_DIR_SRC ${UTILS_DIR}/src)
set(UTILS_DIR_TEST ${UTILS_DIR}/test)

############# 编译 ############# 
# 查找源文件
file(GLOB_RECURSE src_files src/*.cpp)

# 设置链接库输出目录
set(LIBRARY_OUTPUT_PATH ${ROOT_DIR}/lib/utils)

# 设置目标
ADD_LIBRARY(utils ${src_files})
ADD_LIBRARY(utils_so SHARED ${src_files})

# 设置头文件查找目录
# target_include_directories(utils PUBLIC ${UTILS_DIR}/include)
include_directories(
    ${UTILS_DIR_SRC}
    ${UTILS_DIR}/include
)

# 设置链接库搜索目录
link_directories(${ROOT_DIR}/lib/utils)

# 设置依赖的链接库
# target_link_libraries(a PRIVATE b)

# 设置编译选项

# 设置测试目标文件
set_target_properties(utils_so PROPERTIES OUTPUT_NAME utils)
set_target_properties(utils PROPERTIES CLEAN_DIRECT_OUTPUT 1)

############# 测试用例 ############# 
# 启动测试用例
file(GLOB_RECURSE test_src_files ${UTILS_DIR_TEST}/*.cpp)
# set(test_src_files ${UTILS_DIR_TEST}/*.cpp)
set(EXECUTABLE_OUTPUT_PATH ${ROOT_DIR}/test/utils)
add_executable(utils_test ${test_src_files})
target_link_libraries(utils_test PRIVATE utils gtest glog pthread)