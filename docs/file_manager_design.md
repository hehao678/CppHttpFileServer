# CppHttpFileServer FileManager 模块设计文档

---

## 模块简介

FileManager 模块是 CppHttpFileServer 项目核心的文件管理模块，主要负责：

- 接收 HttpServer 模块的文件相关请求
- 实现文件上传（POST）
- 实现文件下载（GET）
- 文件路径管理
- 文件读写错误处理
- 提供文件操作的接口与服务支撑

---

## 模块职责

| 功能分类     | 具体说明                                         |
| ------------ | ------------------------------------------------ |
| 文件上传     | 接收客户端 POST 上传的文件数据，保存到指定目录   |
| 文件下载     | 处理客户端 GET 请求，返回指定文件内容            |
| 文件路径管理 | 对上传和下载路径统一管理，防止非法路径访问       |
| 错误处理     | 文件不存在、读取失败、写入失败，统一返回友好提示 |

---

## 模块依赖关系

HttpServer 模块
|
|---> 调用 FileManager 模块接口
|
|---> 读/写 文件
|---> 错误处理


## 类设计

### 类名：FileManager

| 文件路径               | 内容     |
| ---------------------- | -------- |
| include/file_manager.h | 接口定义 |
| src/file_manager.cpp   | 接口实现 |

---

## 核心接口设计

```cpp
class FileManager {
public:
    // 设置文件根目录
    FileManager(const std::string& basePath);

    // 文件下载接口（GET 请求）
    bool readFile(const std::string& relativePath, std::string& fileContent);

    // 文件上传接口（POST 请求）
    bool writeFile(const std::string& relativePath, const std::string& fileContent);

private:
    std::string basePath_;  // 文件根目录
};
```


## readFile设计流程图

```
readFile(relativePath, fileContent)
│
├── 路径合法性检查（防止目录穿越）
│   └── 有问题 -> 返回 false
│
├── 拼接完整路径 fullPath
│
├── 尝试打开文件
│   └── 打不开 -> Warning日志 + 返回 false
│
├── 读取文件内容到 fileContent
│
├── 打印读取成功日志
│
└── 返回 true
```


## 文件路径规划

| 目录     | 功能                 | 说明               |
| -------- | -------------------- | ------------------ |
| ./files/ | 上传与下载的统一目录 | 文件读写操作根目录 |

> 上传文件存储路径为：
>
> ./files/`<filename>`



## GET 请求处理流程

1. HttpServer 调用 FileManager::readFile()
2. 检查文件是否存在
3. 打开文件进行读取
4. 返回文件内容
5. 失败时返回错误响应（404 Not Found）



## POST 请求处理流程

1. HttpServer 调用 FileManager::writeFile()
2. 检查路径是否合法
3. 打开文件进行写入（覆盖 or 创建）
4. 返回上传成功响应
5. 失败时返回错误响应（500 Server Error）



## 错误处理设计

| 场景         | 返回信息              |
| ------------ | --------------------- |
| 文件不存在   | HTTP 404 Not Found    |
| 文件读写失败 | HTTP 500 Server Error |
| 路径非法访问 | HTTP 403 Forbidden    |


## 安全性设计

### 防止路径穿越攻击

检查 relativePath 是否包含 `../` 等非法路径字符

示例：

```cpp
if (relativePath.find("..") != std::string::npos) {
    // 非法路径
}
```


## 模块拓展点（后续）

| 拓展内容         | 说明             |
| ---------------- | ---------------- |
| 上传文件大小限制 | 防止大文件攻击   |
| 文件分块上传     | 大文件断点续传   |
| 文件删除接口     | DELETE 请求支持  |
| 文件列表查询     | RESTful API 支持 |
| 文件权限控制     | 用户权限管理     |
