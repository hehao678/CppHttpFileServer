cmake_minimum_required(VERSION 3.10)

project(CppHttpFileServer VERSION 1.0 LANGUAGES CXX)

# 指定编译器
set(CMAKE_C_COMPILER /usr/bin/gcc-9)
set(CMAKE_CXX_COMPILER /usr/bin/g++-9)

# 指定 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 开启 compile_commands.json 方便 clangd / lsp
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 开启编译警告（开发时强制）
add_compile_options(-Wall -Wextra -Werror)

# 项目结构打印信息
message(STATUS "Project Source Dir: ${PROJECT_SOURCE_DIR}")
message(STATUS "Compiler Used: ${CMAKE_CXX_COMPILER}")

# 包含头文件
include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

# 查找所有源码文件
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# 打印源码文件
message(STATUS "Source Files: ${SOURCES}")

# 生成可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 链接 pthread 库（使用多线程时必须）
target_link_libraries(${PROJECT_NAME}
    pthread
)

