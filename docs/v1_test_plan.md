# CppHttpFileServer V1 版本测试说明文档

---

## 测试目标

验证 V1 版本的基础功能是否正确，包括：

- HTTP 文件上传（POST）
- HTTP 文件下载（GET）
- 多线程请求处理（ThreadPool）
- 日志模块功能
- 整体稳定性与功能正确性

---

## 测试环境

| 环境项   | 说明                   |
| -------- | ---------------------- |
| 操作系统 | Linux / Ubuntu 20.04+  |
| 编译器   | g++ 7/8/9 (支持 C++17) |
| 构建工具 | cmake >= 3.10          |
| 依赖     | 无第三方依赖，仅标准库 |

---

## 测试目录规范

```
CppHttpFileServer/
├── files/        # 上传 / 下载 文件存放目录
├── logs/         # 日志文件目录
├── build/        # 构建目录
└── CppHttpFileServer # 编译后的可执行文件
```



## 测试前准备

1. 确保 `files/` 目录存在（程序会自动创建）。
2. 确保 `logs/` 目录存在（程序会自动创建）。
3. 启动程序：

```bash
cd build
./CppHttpFileServer
```




## 测试方案与用例

### 测试用例1：测试 GET 文件下载

1. 手动在 `files/` 目录下创建文件：

```bash
echo "Hello HttpServer" > files/test.txt
```

1. 测试下载：

```bash
curl http://127.0.0.1:8080/test.txt -o download.txt
```

1. 验证：

```bash
cat download.txt
# 结果应为：
Hello HttpServer
```



### 测试用例2：测试 POST 文件上传

1. 准备本地文件： `echo "Upload Test File" > upload.txt`
2. 上传文件
