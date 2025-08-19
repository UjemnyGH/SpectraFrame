#include "sfThreadPoolWorker.h"
#include "sfLogger.h"

sf::ThreadWorker::ThreadWorker(int priority) {
  mPriority = priority;
}

int sf::ThreadWorker::getPriority() const {
  return mPriority;
}

bool sf::ThreadWorker::isThreadWorkerRunning() const {
  return running.load();
}

bool sf::ThreadWorkerCompare::operator()(ThreadWorker* a, ThreadWorker* b) {
  return a->getPriority() < b->getPriority();
}

void sf::ThreadPool::_workerThreadRunner() {
  while (true) {
    ThreadWorker* task = nullptr;

    std::unique_lock lock(mThreadQueueMutex);

    mThreadCondition.wait(lock, [this](){ return mThreadsStopping || !mThreadTaskQueue.empty(); });

    if(mThreadsStopping && mThreadTaskQueue.empty())
      return;

    if(mThreadTaskQueue.empty())
      continue;

    task = mThreadTaskQueue.top();
    mThreadTaskQueue.pop();

    lock.unlock();

    if(task) {
      task->running.store(true, std::memory_order_release);
      task->executeFunction();
      task->running.store(false, std::memory_order_release);
    }
  }
}

sf::ThreadPool::ThreadPool(const uint32_t maxThreads) : mMaxActiveThreads(maxThreads == 0 ? std::thread::hardware_concurrency() : maxThreads) {
  for(uint32_t i = 0; i < mMaxActiveThreads; i++) {
    mThreadPool.emplace_back(&ThreadPool::_workerThreadRunner, this);
  }
}

void sf::ThreadPool::stop() {
  std::unique_lock lock(mThreadQueueMutex);

  for(uint32_t i = 0; i < mThreadTaskQueue.size(); i++) {
    mThreadTaskQueue.pop();
  }

  mThreadsStopping.store(true);

  lock.unlock();

  mThreadCondition.notify_all();

  for(std::thread& t : mThreadPool) {
    if(t.joinable())
      t.join();
  }
}

void sf::ThreadPool::addTask(ThreadWorker* pWorker) {
  std::unique_lock lock(mThreadQueueMutex);

  if(mThreadsStopping)
    SF_CLOG("ERR: ThreadPool for " << mMaxActiveThreads << " threads stopped");

  mThreadTaskQueue.push(pWorker);

  lock.unlock();

  mThreadCondition.notify_one();
}
