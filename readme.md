# C++ HTTP 文件传输服务器

## 项目简介

本项目是一个基于 C++ 开发的 HTTP 协议文件传输服务器，主要用于实现简单的文件上传和下载功能，方便在局域网或广域网环境下进行文件共享和传输。

---

## 技术栈

- C++17
- CMake
- 基于 HTTP 协议
- 可选网络库：
  - Boost.Asio
  - cpp-httplib（轻量）
  - 自定义实现（学习网络编程）

---

## 目录结构

```
CppHttpFileServer/├── CMakeLists.txt        # cmake构建配置文件
├── README.md             # 项目说明文档
├── .gitignore            # 忽略文件配置
├── include/              # 存放公共头文件
├── src/                  # 存放源代码
│   ├── main.cpp          # 程序入口
│   ├── http_server.h     # HTTP服务器类定义
│   └── http_server.cpp   # HTTP服务器类实现
├── config/               # 存放配置文件（可选）
│   └── server.conf       # 服务器配置文件（预留）
└── test/                 # 单元测试或测试代码
    └── test_main.cpp     # 测试入口
```



## 功能规划

| 模块        | 功能说明                   | 状态   |
| ----------- | -------------------------- | ------ |
| HTTP Server | 监听HTTP请求，处理GET/POST | 开发中 |
| 文件传输    | 文件上传与下载接口         | 开发中 |
| 配置管理    | 从配置文件读取参数         | 预留   |
| 日志记录    | 打印日志与错误信息         | 预留   |

---

## 编译 & 运行

### 编译

```bash
mkdir build
cd build
cmake ..
make
```
