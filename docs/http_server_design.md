# CppHttpFileServer HttpServer 模块设计文档

---

## 模块简介

HttpServer 模块是 CppHttpFileServer 项目的核心模块，负责：

- 搭建 HTTP 服务端框架
- 监听指定 IP 和端口
- 接收客户端连接
- 解析 HTTP 请求
- 根据请求类型分发给对应的处理函数（GET / POST）
- 调用 FileManager 模块完成文件上传与下载
- 多线程处理请求（借助 ThreadPool 模块）

---

## 技术要点

- 基于 Linux Socket 编程
- TCP 长连接（支持 Keep-Alive 可扩展）
- 简单的 HTTP 协议解析
- 多线程请求处理（线程池）

---

## HttpServer 模块职责

| 功能     | 说明                             |
| -------- | -------------------------------- |
| 初始化   | 绑定 IP 和端口，启动监听         |
| 接收请求 | 接受客户端 socket 连接           |
| 解析请求 | 提取 HTTP 请求头、请求方法、路径 |
| 请求分发 | 根据 GET / POST 调用对应处理函数 |
| 错误处理 | 返回简单的 HTTP 响应码           |
| 资源释放 | 关闭 socket 连接                 |

---

## 核心类设计

### 类名：HttpServer

文件位置：`include/http_server.h   src/http_server.cpp`

## 接口设计（头文件）

```cpp
class HttpServer {
public:
    HttpServer(const std::string& ip, int port);
    ~HttpServer();

    void start();  // 启动服务器主流程

private:
    int server_fd;  // 监听 socket fd
    std::string server_ip;
    int server_port;

    void handleClient(int client_fd);  // 处理客户端请求
    void parseRequest(int client_fd, std::string& method, std::string& path);  // 简单解析请求
    void handleGet(int client_fd, const std::string& path);    // 处理 GET
    void handlePost(int client_fd, const std::string& path);   // 处理 POST
};
```

## 流程示意图

```
main.cpp
    |
    |---> HttpServer.start()
              |
              |---> socket/bind/listen
              |
              |---> while(accept)
                       |
                       |---> ThreadPool.addTask(handleClient)
                                |
                                |---> parseRequest
                                |
                                |---> handleGet / handlePost
```

---

## 响应示例（GET）

请求：` GET /download/test.txt HTTP/1.1`

返回：

```
HTTP/1.1 200 OK
Content-Type: text/plain
<文件内容>
```

## 响应示例（POST）

请求：

```
POST /upload HTTP/1.1
Content-Length: xxxx
<上传的文件内容>
```

返回：

```
HTTP/1.1 200 OK
File uploaded successfully
```

## 模块拓展点（后续）

| 方向               | 说明                   |
| ------------------ | ---------------------- |
| 完善 HTTP 协议解析 | 支持更多请求头解析     |
| RESTful API        | 设计标准 API 接口风格  |
| HTTPS 支持         | 后续可拓展 TLS 加密    |
| 连接复用优化       | 支持长连接、keep-alive |

## 本项目 HttpServer 模块扩展点

| 可拓展方向             | 说明                              |
| ---------------------- | --------------------------------- |
| 支持 HTTPS             | SSL/TLS 加密                      |
| 支持 Keep-Alive        | HTTP 长连接                       |
| 支持更多请求头解析     | Content-Type, Host, User-Agent 等 |
| 使用线程池优化并发处理 | 多线程提升性能                    |
| 优化日志输出           | 更加详细的日志记录                |
| RESTful API 规范设计   | 设计标准 API 风格                 |
