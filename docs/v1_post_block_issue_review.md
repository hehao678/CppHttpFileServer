# CppHttpFileServer V1 版本 POST 请求阻塞问题复盘文档

---

## 问题背景

### 现象描述

- 使用 curl 发送 POST 请求上传文件：

```
curl -X POST --data-binary @upload.txt http://127.0.0.1:8080/upload.txt
```

- 出现现象：
  - curl 卡住，长时间无响应
  - 日志显示：

```
Request Method: POST, Path: /upload.txt, Content-Length: 17
```

- 但是 handlePost() 逻辑未及时返回，直到超时或客户端主动断开连接后才结束。

---

## 初始代码逻辑

handlePost()：

```cpp
while (true) {
    int len = recv(client_fd, buffer, sizeof(buffer), 0);
    if (len == 0) {
        break;  // 客户端断开
    }
    oss.write(buffer, len);
}
```

---

## 问题分析

### 问题本质

- HTTP 协议是基于 TCP 流的协议；
- TCP 流是无边界的，recv() 可能一次性读取：
  - 请求头（Header）
  - 空行
  - 请求体（Body）

---

## 问题解决方案

### 正确的 HTTP 协议处理流程

| 阶段           | 处理内容                                                        |
| -------------- | --------------------------------------------------------------- |
| parseRequest() | 读取请求头，解析 Content-Length，同时获取可能多读到的请求体数据 |
| handlePost()   | 读取剩余 Body 数据，按 Content-Length 控制读取长度              |

---

## 复盘总结

### 问题本质

| 问题                  | 根本原因                                           |
| --------------------- | -------------------------------------------------- |
| recv 阻塞不动         | TCP 流式协议，Header + Body 粘在一起，数据已被读走 |
| handlePost() 等待数据 | 未正确传递 parseRequest() 阶段多余的 bodyData      |

---

## 经验总结

### 关键知识点

1. TCP 是流式协议，不保证消息边界。
2. HTTP 协议需解析 Content-Length 决定 Body 长度。
3. parseRequest() 读取头时需保存多读到的 body 数据。
4. handlePost() 读取剩余 body 数据时必须控制长度。

---

## 通用开发规范

| 场景          | 开发建议                   |
| ------------- | -------------------------- |
| socket 编程   | 控制读取长度，防止粘包问题 |
| HTTP 协议处理 | 请求头与请求体分离处理     |
| 日志记录      | 关键字段日志便于调试       |
| 问题复盘      | 坚持规范复盘，沉淀经验     |

---

> 本文档为 CppHttpFileServer V1 版本 POST 阻塞问题的完整复盘与最佳实践总结。适用于项目内知识库、问题复现记录、团队分享或面试经验复盘。
