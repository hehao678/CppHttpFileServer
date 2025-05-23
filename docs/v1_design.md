# CppHttpFileServer - V1 版本设计文档

## 项目简介

本项目是基于 C++ 实现的 HTTP 协议文件传输服务器。
目标是实现一个简单、可扩展、易维护的 HTTP 文件传输服务端。

---

## 第一版本（V1）设计目标

### 技术栈

- C++17
- CMake 构建
- 自定义简单 HTTP Server
- 多线程处理请求（线程池）
- 文件上传 / 下载功能

---

## V1 核心功能模块

| 模块                  | 文件                    | 说明                            |
| --------------------- | ----------------------- | ------------------------------- |
| HttpServer            | http_server.h / .cpp    | HTTP 服务端，解析请求，管理连接 |
| FileManager           | file_manager.h / .cpp   | 文件读写操作，上传/下载处理     |
| ThreadPool            | thread_pool.h / .cpp    | 简单线程池，处理并发请求        |
| ConfigManager（预留） | config_manager.h / .cpp | 读取配置文件 server.conf        |
| 日志模块              | log.h / .cpp            | 简单日志打印（控制台）          |

---

## V1 支持的功能

### 1. 启动 HTTP 服务

- 指定 IP 和端口
- 监听请求
- 日志打印服务启动信息

---

### 2. GET 请求（下载文件）

接口：GET /download/filename HTTP/1.1

功能：

- 下载服务器指定目录（./files/）的文件
- 返回 HTTP 响应头 + 文件内容

---

### 3. POST 请求（上传文件）

接口：

POST /upload HTTP/1.1


功能：

- 接收客户端上传的文件内容
- 存储到服务器的 ./files/ 目录
- 返回上传成功信息

---

### 4. 多线程处理请求

- 自定义线程池
- 每个连接交给线程池的 worker 线程处理
- 提高并发性能

---


## 后续扩展方案（V2 及以后）

| 模块         | 功能                        | 优先级 |
| ------------ | --------------------------- | ------ |
| 配置管理     | 支持 server.conf 自定义配置 | 高     |
| RESTful API  | 文件列表、删除接口          | 高     |
| 权限控制     | Token 认证、IP 黑白名单     | 中     |
| 前端页面     | 上传下载页面（简单 HTML）   | 中     |
| 日志系统优化 | 文件日志、日志等级          | 中     |
| 文件断点续传 | Range 支持、大文件传输优化  | 中     |
| HTTPS 支持   | 加密传输                    | 低     |
| 压缩传输     | gzip 传输优化               | 低     |

## 备注

V1 版本目标：

> 保证项目能编译、能运行、能传输文件（上传/下载）、支持多线程。

V2 版本之后：

> 增强安全性、可用性、扩展性。

---
