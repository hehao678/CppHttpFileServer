# CppHttpFileServer ThreadPool 模块设计文档

---

## 模块简介

ThreadPool（线程池）模块是 CppHttpFileServer 项目中的高性能并发处理模块。
用于统一管理线程资源，避免频繁创建销毁线程，提高服务器的并发处理能力和稳定性。

---

## 模块职责

| 功能分类 | 具体说明                         |
| -------- | -------------------------------- |
| 线程复用 | 固定数量线程持续运行，等待任务   |
| 任务投递 | 支持将任务添加到线程池中执行     |
| 并发处理 | 多个任务并行执行，提高响应速度   |
| 线程安全 | 线程池内部数据结构保证互斥和同步 |

---

## 核心设计思想

1. 生产者-消费者模型
2. 使用任务队列（std::queue）
3. 使用互斥锁（std::mutex）
4. 条件变量同步（std::condition_variable）
5. 线程安全的任务调度
6. 每个线程通过 while(true) + wait() 不断获取任务执行

---

## 模块依赖关系


HttpServer 模块
|
|---> ThreadPool.addTask()
|
|---> 任务放入安全队列
|---> 唤醒空闲线程执行任务



## 类设计

### 类名：ThreadPool

| 文件路径              | 内容     |
| --------------------- | -------- |
| include/thread_pool.h | 接口定义 |
| src/thread_pool.cpp   | 接口实现 |

---

## 接口设计（头文件）

```cpp
class ThreadPool {
public:
    explicit ThreadPool(size_t numThreads);  // 初始化线程池
    ~ThreadPool();  // 资源释放

    // 添加任务到线程池
    void addTask(const std::function<void()>& task);

private:
    std::vector<std::thread> workers_;  // 工作线程
    std::queue<std::function<void()>> tasks_;  // 任务队列

    std::mutex mtx_;
    std::condition_variable cv_;
    bool stop_;  // 线程池停止标志

    // 线程主循环
    void workerThread();
};
```


## 线程池核心工作流程

1. 初始化线程池，创建 N 个线程。
2. 所有线程阻塞等待任务。
3. HttpServer 调用 addTask() 投递任务。
4. 被唤醒的线程取出任务执行。
5. 如果 stop_ 为 true，线程退出。
6. 析构函数等待所有线程结束。

## 并发模型示意图

```
addTask()
   |
[任务队列]  <- 多个线程从任务队列取任务执行
   |
workerThread() 循环执行任务
```



## 模块拓展方向（高级版）

| 功能         | 说明                       |
| ------------ | -------------------------- |
| 定时任务     | 线程池延迟执行任务         |
| 动态扩容     | 根据负载动态调整线程数量   |
| 异步结果获取 | std::future 获取任务返回值 |
| 多线程日志   | 日志写入也可以走线程池     |
