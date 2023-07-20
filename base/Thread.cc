#include "Thread.h"

#include <unistd.h>
#include <sys/syscall.h>

#include "CurrentThread.h"
#include "Timestamp.h"
namespace peak {
namespace detail {

pid_t gettid() {
    return static_cast<pid_t>(::syscall(SYS_gettid));
}

//将线程实际运行函数通过参数形式传到pthread_create的route里
struct ThreadData {
    typedef peak::Thread::ThreadFunc ThreadFunc;
    ThreadFunc func_;
    string name_;
    pid_t* tid_;

    ThreadData(ThreadFunc func,
             const string& name,
             pid_t* tid)
    : func_(std::move(func)),
      name_(name),
      tid_(tid)
  { }
    void runInThread() {
        *tid_ = peak::CurrentThread::tid();
        tid_ = NULL;
        func_();
    }
};

void* startThread(void* obj)
{
  ThreadData* data = static_cast<ThreadData*>(obj);
  data->runInThread();
  delete data;
  return NULL;
}

}//namespace detail

/*
对线程全局数据的一些操作
*/
void CurrentThread::cacheTid() {
  if (t_cachedTid == 0)
  {
    t_cachedTid = detail::gettid();
    t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
  }
}

bool CurrentThread::isMainThread() {
  return tid() == ::getpid();
}

void CurrentThread::sleepUsec(int64_t usec) {
  struct timespec ts = { 0, 0 };
  ts.tv_sec = static_cast<time_t>(usec / Timestamp::kMicroSecondsPerSecond);
  ts.tv_nsec = static_cast<long>(usec % Timestamp::kMicroSecondsPerSecond * 1000);
  ::nanosleep(&ts, NULL);
}

//以下才是Thread的成员定义

AtomicInt32 Thread::numCreated_;

Thread::Thread(ThreadFunc func, const string& n)
  : started_(false),
    joined_(false),
    pthreadId_(0),
    tid_(0),
    func_(std::move(func)),
    name_(n)
{
  setDefaultName();
}
Thread::~Thread() {
  if (started_ && !joined_)//如果线程启动了没有被joined则detach回收资源
  {
    pthread_detach(pthreadId_);
  }
}

void Thread::start() {
    started_ = true;
    detail::ThreadData* data = new detail::ThreadData(func_, name_, &tid_);
    if (pthread_create(&pthreadId_, NULL, &detail::startThread, data)) {
        started_ = false;
    }
}


void Thread::setDefaultName() {
  int num = numCreated_.incrementAndGet();
  if (name_.empty())
  {
    char buf[32];
    snprintf(buf, sizeof buf, "Thread%d", num);
    name_ = buf;
  }
}

int Thread::join() {
  joined_ = true;
  return pthread_join(pthreadId_, NULL);
}

}//namespace peak
