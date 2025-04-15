# CppHttpFileServer 项目 V1 版本说明（个人学习项目）

---

## 项目简介

CppHttpFileServer 是本人独立开发的 C++ 后端学习项目，旨在通过实际编码与工程实践，深入理解网络编程、HTTP 协议处理、多线程编程与服务端开发框架的搭建。该项目模拟了一个基础的 HTTP 文件传输服务器，支持文件上传与下载，使用自定义线程池处理并发请求，完整复现了服务端开发的关键模块与流程。

该项目用于巩固 C++ 编程能力，训练服务端工程能力，并为后续大模型 Agent 系统、网络协议开发等方向的深入学习与实战打下基础。

---


## 技术栈

- C++17 / Linux / Socket 编程 / HTTP 协议处理
- 多线程 ThreadPool 模块
- 自定义日志模块（日志等级 / 文件输出）
- CMake 构建管理
- wrk 压力测试工具

## 1.项目目录结构清单

```
CppHttpFileServer/
├── build/                        # CMake 构建目录（自动生成）
│   ├── CMakeCache.txt            # CMake 缓存文件
│   ├── CMakeFiles/               # CMake 生成的中间文件
│   ├── cmake_install.cmake       # CMake 安装脚本
│   ├── compile_commands.json     # 编译数据库（供 clangd 使用）
│   ├── CppHttpFileServer         # 编译生成的可执行程序
│   └── Makefile                  # 编译规则文件
│
├── CMakeLists.txt                # CMake 构建配置文件
│
├── config/                       # 配置文件目录
│   └── server.conf               # 服务器配置文件（如 IP、端口）
│
├── docs/                         # 项目文档目录（预留）
│
├── include/                      # 头文件目录
│   ├── file_manager.h            # 文件管理模块头文件
│   ├── http_server.h             # HttpServer 模块头文件
│   ├── log.h                     # 日志模块头文件
│   └── thread_pool.h             # 线程池模块头文件
│
├── readme.md                     # 项目简介（简要版）
├── README_V1.md                  # V1 版本详细说明文档
│
├── src/                          # 源码目录
│   ├── file_manager.cpp          # 文件管理模块实现
│   ├── http_server.cpp           # HttpServer 模块实现
│   ├── log.cpp                   # 日志模块实现
│   ├── main.cpp                  # 程序入口
│   └── thread_pool.cpp           # 线程池模块实现
│
└── test/                         # 测试目录
    ├── CppHttpFileServer         # 测试环境编译的可执行程序
    ├── download.txt              # 测试用的下载文件
    ├── files/                    # 上传/下载测试的文件存放目录
    ├── logs/                     # 日志输出目录
    ├── run.sh                    # 一键启动测试脚本
    ├── test_main.cpp             # 单元测试代码
    └── upload.txt                # 测试上传的文件

```

---

## 2. 项目构建与启动说明

### 编译环境

- Linux / Ubuntu
- g++ >= 7
- cmake >= 3.10

### 构建步骤

```bash
mkdir build
rm -rf *
cd build
cmake ..
make -j
```

### 启动服务

```bash
cd test
chmod +x run.sh
./run.sh
```

日志输出位置：`./test/logs/server.log`

---

## 3. 功能测试说明

功能测试用例详见：`docs/v1_test_plan.md`

### curl 测试示例

```bash
curl http://127.0.0.1:8080/test.txt -o download.txt
curl -X POST --data-binary @upload.txt http://127.0.0.1:8080/upload.txt
```

---

## 4. 压力测试与并发测试说明

压力测试方案详见：`docs/v1_stress_test_guide.md`

### wrk 测试示例

```bash
wrk -t4 -c100 -d10s http://127.0.0.1:8080/test.txt
wrk -t4 -c50 -d10s -s post.lua http://127.0.0.1:8080/upload.txt
```

---

## 5. V1 项目完整交付物清单

| 交付物       | 说明                          |
| ------------ | ----------------------------- |
| 项目源码     | include/ src/                 |
| 构建配置     | CMakeLists.txt                |
| 测试脚本     | test/ wrk post.lua            |
| 项目文档     | docs/ 各模块设计文档          |
| 项目说明     | README_V1.md                  |
| 测试计划     | v1_test_plan.md               |
| 压力测试方案 | v1_stress_test_guide.md       |
| 问题复盘     | v1_post_block_issue_review.md |

---

## 6. V2 版本功能扩展方案

| 模块        | 计划优化方向                       |
| ----------- | ---------------------------------- |
| HttpServer  | 支持长连接 keep-alive，RESTful API |
| FileManager | 文件删除接口，文件列表查询接口     |
| ThreadPool  | 动态线程池扩容，自定义任务优先级   |
| 日志模块    | 日志异步写入，日志切割与归档       |
| 配置模块    | 外部配置文件 server.conf 支持      |
| 安全性      | 文件大小限制，路径穿越防护优化     |
| 其他特性    | 支持 TLS (HTTPS)，基础认证机制     |

---

 本项目为个人学习项目，全部代码与设计均为自主实现，用于技术积累与能力提升，无任何商业用途。
