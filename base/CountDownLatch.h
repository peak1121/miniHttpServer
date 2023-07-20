#ifndef HTTPSERVER_BASE_COUNTDOWNLATCH_H
#define HTTPSERVER_BASE_COUNTDOWNLATCH_H

#include "Condition.h"
#include "Mutex.h"

namespace peak
{
//用户子线程等待子线程发起“起跑”
//也可以用于主线程等待子线程初始化完毕
class CountDownLatch
{
public:
    explicit CountDownLatch(int count);//构造函数
    void wait();//等待计数减为0
    void countDown();//计数
    int getCount() const;
private:
    mutable MutexLock mutex_;
    Condition condition_ ;
    int count_ ;
};

}  // namespace peak


#endif


