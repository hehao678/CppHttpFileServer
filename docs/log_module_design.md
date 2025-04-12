# CppHttpFileServer 日志模块设计文档

## 模块简介

本模块是 CppHttpFileServer 项目的日志系统，主要用于项目运行过程中日志的记录和输出，方便调试、排查问题以及线上日志分析。

---

## 模块设计目标

### 核心需求

- 自定义日志模块（独立 log.h / log.cpp）
- 单例模式设计，保证全局唯一实例
- 线程安全（mutex 加锁）
- 日志等级控制（DEBUG / INFO / WARNING / ERROR）
- 日志双重输出（控制台 + 文件）

---

## 日志等级定义

| 日志等级 | 用途     | 使用场景                       |
| -------- | -------- | ------------------------------ |
| DEBUG    | 调试日志 | 打印详细的调试数据             |
| INFO     | 信息日志 | 程序正常运行的日志             |
| WARNING  | 警告日志 | 存在潜在问题，但不影响程序运行 |
| ERROR    | 错误日志 | 严重错误，程序需要注意或退出   |

---

## 日志输出要求

### 1. 控制台输出

- 格式：[等级] 时间 [文件名:行号] 日志内容

### 2. 日志文件输出

- 默认路径：./logs/server.log
- 自动创建 logs/ 目录
- 同步输出到控制台和日志文件

### 带变量日志（推荐写法）

采用 `std::ostringstream` 进行日志字符串拼接：

cpp

```
#include <sstream>
int userId = 123;
std::string username = "Alice";

std::ostringstream oss;
oss << "User login, userId = " << userId << ", username = " << username;

LOG_INFO(oss.str());

```

## 不推荐的写法（不支持）

日志模块不支持类似 printf 格式：

`LOG_INFO("userId = %d, username = %s", userId, username); // 错误`

日志模块不支持类似 std::format（C++20特性）：

`LOG_INFO(std::format("userId = {}, username = {}", userId, username)); // 不建议`

---

## 日志模块核心接口设计

| 函数                                                                        | 说明                         |
| --------------------------------------------------------------------------- | ---------------------------- |
| getInstance()                                                               | 获取 Logger 单例实例         |
| setLogLevel(LogLevel level)                                                 | 设置日志等级过滤             |
| setLogFile(const std::string& filename)                                     | 设置日志输出文件路径         |
| log(LogLevel level, const std::string& message, const char* file, int line) | 日志输出函数                 |
| getCurrentTime()                                                            | 获取当前时间字符串（格式化） |
| logLevelToString(LogLevel level)                                            | 日志等级转换为字符串         |

---

## 日志宏定义

便于在代码中直接调用日志打印，自动带上文件名和行号。

```cpp
#define LOG_DEBUG(msg) Logger::getInstance().log(LogLevel::DEBUG, msg, __FILE__, __LINE__)
#define LOG_INFO(msg) Logger::getInstance().log(LogLevel::INFO, msg, __FILE__, __LINE__)
#define LOG_WARNING(msg) Logger::getInstance().log(LogLevel::WARNING, msg, __FILE__, __LINE__)
#define LOG_ERROR(msg) Logger::getInstance().log(LogLevel::ERROR, msg, __FILE__, __LINE__)
```

## 模块优点

* 易用性好，接口友好
* 单例模式，方便全局日志调用
* 支持日志等级过滤，减少日志冗余
* 支持日志文件持久化，方便线上问题排查
* 线程安全设计，保证并发日志输出正确

---

## 后续可拓展功能

| 拓展项       | 说明                        |
| ------------ | --------------------------- |
| 日志文件切割 | 按日期/大小自动切分日志文件 |
| 异步日志     | 使用异步队列提高性能        |
| 多日志文件   | 不同模块单独记录日志        |
| 日志颜色高亮 | 控制台日志颜色区分日志等级  |



## 代码说明


### 单例模块设计


```
Logger(constLogger&) =delete;  
Logger&operator=(constLogger&) =delete;
```

这两行代码的作用是： 	禁止对象的拷贝和赋值。

因为日志模块的设计是 单例模式（Singleton Pattern）

#### 单例模式的核心思想：

* 一个类在整个程序生命周期中，只能有一个实例。
* 不能被随意复制或者赋值，否则就破坏了单例的约束。

#### 扩展知识点

这种写法是 C++11 引入的新特性：

> = delete

代表：

* 告诉编译器禁止这类函数的调用
* 明确禁止拷贝 / 赋值
* 比以前的 private 声明更直接和规范
