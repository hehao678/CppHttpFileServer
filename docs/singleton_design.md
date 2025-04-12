# C++ 单例模式设计总结

---

## 单例模式（Singleton Pattern）

### 定义

单例模式是一种创建型设计模式，目的是保证一个类在整个程序生命周期中只有一个实例，并提供全局访问点。

---

## 应用场景

- 日志系统（Logger）
- 配置管理器（ConfigManager）
- 数据库连接池（DBPool）
- 线程池（ThreadPool）

---

## 核心设计思想

1. 构造函数私有化
2. 禁止拷贝构造和赋值操作
3. 提供一个静态的 getInstance() 接口获取唯一实例
4. 保证线程安全（必要时）

---

## 现代 C++（C++11及以后）标准写法

### 头文件示例（Logger为例）

```cpp
#ifndef LOG_H
#define LOG_H

class Logger {
public:
    static Logger& getInstance(); // 获取单例实例

    void log(const std::string& message); // 日志输出示例函数

private:
    Logger() = default;                          // 私有构造
    Logger(const Logger&) = delete;              // 禁止拷贝构造
    Logger& operator=(const Logger&) = delete;   // 禁止赋值操作
};

#endif // LOG_H
```


### 源文件实现

```cpp
#include "log.h"
#include <iostream>

Logger& Logger::getInstance() {
    static Logger instance;  // 静态局部变量，程序运行期间只创建一次
    return instance;
}

void Logger::log(const std::string& message) {
    std::cout << "[LOG] " << message << std::endl;
}
```


总结的话是两种实现；一种是利用static；转换成静态变量

另一个就是禁止拷贝构造和赋值等操作；使用=delete操作
