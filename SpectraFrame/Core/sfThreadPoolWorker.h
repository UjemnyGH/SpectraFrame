#pragma once
#ifndef _SPECTRAFRAME_THREAD_POOL_WORKER_
#define _SPECTRAFRAME_THREAD_POOL_WORKER_

#include <mutex>
#include <atomic>
#include <thread>
#include <vector>
#include <condition_variable>
#include <queue>
#include <functional>

namespace sf {
  class ThreadWorker {
  private:
    int mPriority;

  public:
    std::atomic<bool> running{false};
    std::function<void()> executeFunction;

    ThreadWorker(int priority);

    int getPriority() const;

    bool isThreadWorkerRunning() const;
  };

  struct ThreadWorkerCompare {
    bool operator()(ThreadWorker* a, ThreadWorker* b);
  };

  class ThreadPool {
  private:
    std::vector<std::thread> mThreadPool;
    std::priority_queue<ThreadWorker*, std::vector<ThreadWorker*>, ThreadWorkerCompare> mThreadTaskQueue;

    std::mutex mThreadQueueMutex;
    std::condition_variable mThreadCondition;
    std::atomic<bool> mThreadsStopping{false};

    uint32_t mMaxActiveThreads;

    void _workerThreadRunner();

  public:
    ThreadPool(const uint32_t maxThreads);

    void stop();

    void addTask(ThreadWorker* pWorker);
  };
}

#endif
