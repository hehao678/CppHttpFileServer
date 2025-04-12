#include "thread_pool.h"
#include "log.h"

// 创建线程池
ThreadPool::ThreadPool(size_t numThreads) : stop_(false)
{
    for (size_t i = 0; i < numThreads; i++)
    {
        workers_.emplace_back([this]() { workerThread(); });
    }
    LOG_INFO("ThreadPool created with " + std::to_string(numThreads) + " threads.");
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(mtx_);
        stop_ = true;
    }
    cv_.notify_all(); //唤醒所有线程

    for (auto &worker : workers_)
    {
        if (worker.joinable())
        {
            worker.join(); //等待所有线程结束
        }
    }
    LOG_INFO("ThreadPool destroyed,all threads exited.");
}

void ThreadPool::addTask(const std::function<void()> &task)
{
    {
        std::unique_lock<std::mutex> lock(mtx_);
        task_.push(task); //将任务放入队列
    }
    cv_.notify_one(); //唤醒一个空闲线程
}

void ThreadPool::workerThread()
{
    while (true)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(mtx_);
            cv_.wait(lock, [this]() { return stop_ || !task_.empty(); });
            if (stop_ && task_.empty())
            {
                return; //退出线程
            }
            task = task_.front();
            task_.pop();
        }
        task(); // 执行任务
    }
}