## 代码核心逻辑梳理

| 代码位置                   | 功能                   | 知识点                                      |
| -------------------------- | ---------------------- | ------------------------------------------- |
| socket()                   | 创建 socket 文件描述符 | Linux 网络编程                              |
| bind()                     | 绑定 IP 和端口         | socket 地址绑定                             |
| listen()                   | 监听连接请求           | TCP 半连接队列                              |
| accept()                   | 接收客户端连接         | 阻塞 / 非阻塞模型                           |
| recv()                     | 接收客户端数据         | TCP 粘包 / 拆包问题                         |
| send()                     | 发送响应数据           | TCP 保证顺序 / 确认机制                     |
| parseRequest()             | 简单的 HTTP 请求解析   | HTTP 协议结构：请求行、请求头、空行、请求体 |
| handleGet() / handlePost() | 请求类型处理           | 请求方法分发（GET / POST）                  |

### 1. socket 编程的基本流程？

```text
socket() -> bind() -> listen() -> accept() -> recv()/send() -> close()
```

### 2. TCP 和 UDP 区别？

| TCP                      | UDP          |
| ------------------------ | ------------ |
| 面向连接（三次握手）     | 无连接       |
| 可靠传输（顺序、无丢包） | 不保证可靠   |
| 慢                       | 快（无状态） |

### 3. TCP 粘包 / 拆包问题是什么？

原因：

* TCP 是流式协议，无消息边界
* 发送端 write 连续多次，接收端 recv 一次性收到多个包

解决方案：

* HTTP 协议通过 Content-Length 明确消息长度
* 自定义协议中通过定长包头、特殊分隔符、长度字段来拆包

### 4.HTTP 协议的基本格式？

```

<请求行>
GET /path HTTP/1.1

<请求头>
Content-Length: xxx

<空行>

<请求体>
xxxxxx
```

### 6. 面试官可能会深入的问题

| 问题                         | 知识考察点                       |
| ---------------------------- | -------------------------------- |
| TCP 三次握手和四次挥手流程？ | TCP 协议原理                     |
| socket 如何设置非阻塞？      | fcntl 系统调用                   |
| 多线程下如何处理多个连接？   | select/poll/epoll 模型 或 线程池 |
| HTTP 如何支持大文件传输？    | 分块传输 / Range 请求头          |

### 5. 为什么 HttpServer 采用类封装？

* 高内聚，低耦合
* 便于模块化设计
* 易于后续扩展（如支持 HTTPS / RESTful API）

## HTTPServer相关问题记录

* http协议的主要组成是什么样的？
* 是如何获取http协议相关的内容的？

## FileManager相关问题记录

* explicit关键字的作用是什么？？
* *路径安全检查* （Path Security Check）逻辑
* 

### 路径安全检查

一般文件的读取：readFile(const std::string& relativePath)

因该先检查一下传入的 `relativePath` 是否包含了 `..`

```
/path/to/wwwroot/file.txt         # 正常路径
/path/to/wwwroot/../../etc/passwd # 目录穿越攻击
```

**代码说明**

```cpp
// 判断路径中是否存在 ".."
if (relativePath.find("..") != std::string::npos)
```

说明：

* `std::string::find` 用于查找子串。
* 返回值：
  * 找到 -> 返回位置（>= 0）
  * 没找到 -> 返回 `std::string::npos`

### 建议优化（更安全做法）

目前这段代码是基础的路径检查。更完善的做法可以是：

#### 1. 检查绝对路径

拼接出完整路径后，检查是否在指定根目录：

```cpp
std::filesystem::path fullPath = basePath / relativePath;
fullPath = std::filesystem::weakly_canonical(fullPath);

if (fullPath.string().find(basePath.string()) != 0)
{
    LOG_WARNING("Path escapes base directory.");
    return false;
}
```

#### 2. 正则限制路径字符

只允许字母、数字、下划线、斜杠等安全字符。

## 线程池相关问题

* 添加任务到线程池的函数原理：`voidaddTask(conststd::function<void()> &task);`
* 线程池的创建: `workers_.emplace_back(`
* 线程池的析构函数代码理解
* 线程池中addTask()原理理解
* notify_one()和notify_all()原理以及区别
* 

## git操作相关问题

* 如何将不同的版本创建不同的分支？
* 如何使用SSH key登录github?使用git push经常出现403 error
* 




## 第一版测试问题

curl 上传时一直卡住、不返回、不报错，原因非常明确：

### 问题本质

你的 HttpServer 没有正确读取完整的 HTTP 请求 Body 数据（文件内容），导致：

* 客户端（curl）在等待服务器继续接收数据；
* 服务器（你的 handlePost 函数）执行完一次 recv 后直接 return 或处理数据；
* TCP 连接还没断，curl 以为服务器没接收完，继续等待写入窗口；


### 当前代码

```cpp
int len = recv(client_fd, buffer, sizeof(buffer), 0);

if (len <= 0)
{
    // 错误处理
}

std::string content(buffer, len);

// 直接写入文件
fileManager_.writeFile(path, content);
```


问题：只读了一次 recv，但 curl 发送的文件数据远远超过了 buffer 的大小（4096），剩余数据还在传输过程中。

而你提前返回并发了 HTTP 200 响应，curl 发现 TCP 连接没断，也没收到完整的 response，就一直卡住等待。

### 正确的处理方式

必须：

1. 解析 Content-Length（推荐）
2. 或者 不断循环 recv，直到 recv 返回 0（客户端关闭连接）
