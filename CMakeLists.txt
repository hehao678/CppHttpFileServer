cmake_minimum_required(VERSION 3.10)

project(CppHttpFileServer VERSION 1.0 LANGUAGES CXX)

# 强制指定高版本编译器
set(CMAKE_C_COMPILER /usr/bin/gcc-9)
set(CMAKE_CXX_COMPILER /usr/bin/g++-9)

# C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 导出 compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 包含头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 查找源文件
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# 生成可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 编译警告（开发推荐开启）
add_compile_options(-Wall -Wextra -Werror)

message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message(STATUS "SOURCES: ${SOURCES}")
message(STATUS "CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
