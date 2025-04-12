

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool
{
  public:
    //构造函数；初始化线程池，创建numThreads个线程
    explicit ThreadPool(size_t numThreads);
    //析构函数
    ~ThreadPool();

    // 添加任务到线程池
    void addTask(const std::function<void()> &task);

  private:
    std::vector<std::thread>          workers_; //工作线程的集合
    std::queue<std::function<void()>> task_;    //任务队列

    std::mutex              mtx_;  //互斥锁保护任务队列
    std::condition_variable cv_;   //条件变量同步
    bool                    stop_; //线程池是否停止

    // 每个工作线程执行的主循环
    void workerThread();
};

#endif // THREAD_POOL_H