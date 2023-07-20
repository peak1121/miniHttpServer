#include "base/ThreadPool.h"
#include "base/CurrentThread.h"
#include "base/Mutex.h"
#include <string>
#include <stdio.h>
#include <unistd.h>
void threadFunc()
{
  printf("tid=%d\n", peak::CurrentThread::tid());
}
class Compute {
    public : 
    Compute() : sum_(0), mutex_(){}
    int sum(){return sum_;}
    void accumulateNtoM (int n, int m) {
        int localSum = 0;
        for(int i = n; i <= m; ++i) {
            localSum += i;
        }
        // printf("结果是%d\n", localSum);
        peak::MutexLockGuard lock(mutex_);
        sum_ += localSum;
    }
    private : 
    int sum_;
    peak::MutexLock mutex_; 
};
int main(int argc, char const *argv[])
{
    peak::ThreadPool pool("theadPool");
    pool.setMaxQueueSize(10);//设置最大任务队列
    pool.setThreadInitCallback([](){});
    pool.start(5);

    printf("pid=%d, tid=%d\n", ::getpid(), peak::CurrentThread::tid());
    Compute coo;
    pool.run(std::bind(&Compute::accumulateNtoM, &coo, 1, 100000));
    pool.run(std::bind(&Compute::accumulateNtoM, &coo, 100001, 200000));
    pool.run(std::bind(&Compute::accumulateNtoM, &coo, 200001, 300000));
    pool.run(std::bind(&Compute::accumulateNtoM, &coo, 300001, 400000));
    pool.run(std::bind(&Compute::accumulateNtoM, &coo, 400001, 500000));
    sleep(1);
    printf("结果是%d\n", coo.sum());


    return 0;
}
