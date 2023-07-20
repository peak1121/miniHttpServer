#ifndef HTTPSERVER_BASE_CONDITION_H
#define HTTPSERVER_BASE_CONDITION_H

#include <pthread.h>

#include "Mutex.h"
namespace peak
{

class Condition
{
public:
    explicit Condition(MutexLock& mutex) : mutex_(mutex)
    {
      pthread_cond_init(&pcond_, NULL);
    }
    ~Condition()
    {
      pthread_cond_destroy(&pcond_);
    }
    void wait()
    {
      MutexLock::UnassignGuard ug(mutex_);//ug生命周期结束时获取到锁并设置线程
      pthread_cond_wait(&pcond_, mutex_.getPthreadMutex());
    }
    bool waitForSeconds(double seconds);
    void notify()
    {
      pthread_cond_signal(&pcond_);
    }
    void notifyAll()
    {
      pthread_cond_broadcast(&pcond_);
    }
private:
    MutexLock& mutex_;//条件变量需要配合锁使用
    pthread_cond_t pcond_;
};

}  // namespace peak

#endif


