# CppHttpFileServer V1 版本 压力测试与多线程并发测试说明文档

---

## 1. 测试目的

验证 CppHttpFileServer 在高并发请求场景下的性能表现、稳定性以及线程池模块的正确性。

---

## 2. 测试原理

### 2.1 并发与压力测试基本概念

| 名称                         | 说明                                               |
| ---------------------------- | -------------------------------------------------- |
| 并发测试（Concurrency Test） | 模拟大量客户端同时访问服务器，测试多线程处理能力   |
| 压力测试（Stress Test）      | 超过系统最大负载的测试，检测服务极限与异常处理能力 |

---

### 2.2 HttpServer 多线程处理模型

本项目 HttpServer 模块采用 ThreadPool 处理并发请求：

```
主线程 (accept)
   |
线程池 ThreadPool
   |
Worker Thread 多线程执行 handleClient()
```

---

## 3. 常用测试工具

| 工具                  | 说明                                          | 推荐用途 |
| --------------------- | --------------------------------------------- | -------- |
| wrk                   | 高性能 HTTP 压力测试工具（支持多线程+多连接） | 推荐     |
| ab (Apache Benchmark) | 简单易用的压力测试工具                        | 基础测试 |
| siege                 | HTTP 压测工具，支持配置文件                   | 可选     |

---

## 4. wrk 工具介绍

### 安装

```bash
sudo apt install wrk
```

直接使用这个方法不行，需要下载库自行编译进行使用，操作指南如下：

下载库 `git clone https://github.com/wg/wrk.git`

或者直接采用ssh的方式下载  `git clone git@github.com:wg/wrk.git`

进入到wrk目录  `cd wrk`

使用make进行编译  `make`

安装 wrk（移动到全局命令路径）  `sudo cp wrk /usr/local/bin/`

测试是否安装成功  `wrk --version`

一般输出

```
wrk 4.2.0 [epoll] Copyright (C) 2012 Will Glozer
Usage: wrk <options> <url>                        
  Options:                                        
    -c, --connections <N>  Connections to keep open   
    -d, --duration    <T>  Duration of test       
    -t, --threads     <N>  Number of threads to use   
                                                  
    -s, --script      <S>  Load Lua script file   
    -H, --header      <H>  Add header to request  
        --latency          Print latency statistics   
        --timeout     <T>  Socket/request timeout   
    -v, --version          Print version details  
                                                  
  Numeric arguments may include a SI unit (1k, 1M, 1G)
  Time arguments may include a time unit (2s, 2m, 2h)
```


### 常用命令

```bash
wrk -t4 -c100 -d10s http://127.0.0.1:8080/test.txt
```

含义：

| 参数  | 说明                             |
| ----- | -------------------------------- |
| -t4   | 使用4个线程发起请求              |
| -c100 | 保持100个 TCP 并发连接           |
| -d10s | 测试时长10秒                     |
| url   | 测试目标地址（你的 HTTP Server） |

---

## 5. 测试方案

### 测试场景1：基础并发 GET 请求

```bash
wrk -t4 -c100 -d10s http://127.0.0.1:8080/test.txt
```

### 测试场景2：模拟高并发上传 POST 请求

准备上传数据文件：

```bash
echo "UploadTestData" > upload.txt
```

wrk + 自定义 POST 请求：

```bash
wrk -t4 -c50 -d10s -s post.lua http://127.0.0.1:8080/upload.txt
```

post.lua 脚本内容：

```lua
wrk.method = "POST"
wrk.body   = "UploadTestData"
wrk.headers["Content-Type"] = "text/plain"
```

---

## 6. 测试成功标准

| 测试内容     | 判断标准                              |
| ------------ | ------------------------------------- |
| 服务稳定性   | wrk 过程中无崩溃、无错误日志          |
| 多线程正确性 | 日志显示并发请求处理正常              |
| 性能指标     | Requests/sec 符合预期（根据机器性能） |
| 日志记录     | 正确记录每个请求的日志                |



模拟测试结果输出

```bash
(base) ubuntu@ubuntu:/mnt/sdb/hh/hhe_cpp/Prj/CppHttpFileServer/test$ wrk -t4 -c100 -d10s http://127.0.0.1:8080/test.txt
Running 10s test @ http://127.0.0.1:8080/test.txt
  4 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    23.91ms   25.49ms 102.79ms   80.64%
    Req/Sec     1.38k   484.11     3.43k    70.25%
  54953 requests in 10.01s, 4.30MB read
  Socket errors: connect 0, read 54952, write 0, timeout 0
Requests/sec:   5489.46
Transfer/sec:    439.59KB
```




---

## 7. 测试注意事项

- 压力测试环境应与实际生产隔离。
- 注意限制日志等级，防止大量日志影响性能。
- 日志可统计请求数、错误率。
- 可扩展对大文件、长连接、异常请求的测试。

---

## 8. 后续可拓展测试内容

| 测试方向               | 说明                              |
| ---------------------- | --------------------------------- |
| 长连接 keep-alive 测试 | 检测连接复用能力                  |
| 大文件上传/下载测试    | 检测分块传输能力                  |
| HTTP/HTTPS 测试        | 安全协议支持                      |
| 恶意请求测试           | SQL注入 / XSS / Path Traversal 等 |

---

> 本文档为 CppHttpFileServer 项目 V1 版本 压力测试与多线程并发测试规范文档，用于开发自测、运维测试、性能优化及面试知识总结。
