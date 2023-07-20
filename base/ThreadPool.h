#ifndef HTTPSERVER_BASE_THREADPOOL_H
#define HTTPSERVER_BASE_THREADPOOL_H

#include "base/Condition.h"
#include "base/Mutex.h"
#include "base/Thread.h"
#include "base/Types.h"

#include <deque>
#include <vector>

namespace peak
{

class ThreadPool
{
 public:
  typedef std::function<void ()> Task;

  explicit ThreadPool(const string& nameArg = string("ThreadPool"));
  ~ThreadPool();//析构函数要唤醒等待的线程并等待他们结束

  // Must be called before start().
  void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; }
  void setThreadInitCallback(const Task& cb)//设置线程池创建之前的初始化函数
  { threadInitCallback_ = cb; }

//启动线程池，创建numThreads个线程并启动，线程运行函数为runThread
  void start(int numThreads);
  void stop();

  const string& name() const
  { return name_; }

  size_t queueSize() const;

  //线程池执行任务，没有线程池为空则由主线程执行，如果有就利用条件变量唤醒等待中的线程
  void run(Task f);

 private:
  bool isFull() const ;
  //线程执行函数，当有任务时就取出任务执行
  void runInThread();
  //从任务队列取出一个队列
  Task take();

  //保护任务队列
  mutable MutexLock mutex_;//互斥量用于条件变量
  Condition notEmpty_ ;//通知任务队列不为空
  Condition notFull_;//通知任务队列不为满

  string name_;//线程池名字
  Task threadInitCallback_;//线程池初始化函数
  std::vector<std::unique_ptr<peak::Thread>> threads_;//维护当前线程池，每一个用智能指针维护
  std::deque<Task> queue_;//任务队列
  size_t maxQueueSize_;
  bool running_;
};

}  // namespace peak

#endif
