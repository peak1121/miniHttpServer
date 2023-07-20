#ifndef HTTPSERVER_BASE_THREAD_H
#define HTTPSERVER_BASE_THREAD_H
//代码规范：所有头文件都应该有 #define 保护来防止头文件被多重包含, 命名格式当是: <PROJECT>_<PATH>_<FILE>_H_
/*
包含顺序如下
本cc对应的头文件优先位置, 详情如下)
C 系统文件
C++ 系统文件
其他库的 .h 文件
本项目内 .h 文件
*/
#include <pthread.h>

#include <functional>
#include <memory>
#include <string>

#include "Types.h"
#include "Atomic.h"
#include "CountDownLatch.h"
namespace peak {

class Thread {
public:

    typedef std::function<void ()> ThreadFunc;
    
    explicit Thread(ThreadFunc, const string& name = string());
    ~Thread();

    void start();
    int join();

    bool started() const { return started_; }
    pthread_t pthreadId() const { return pthreadId_; }
    pid_t tid() const { return tid_; }
    const string& name() const { return name_; }

    static int numCreated() { return numCreated_.get(); }
private:

    void setDefaultName();//设置默认名字为thread i

//代码规范：成员遍历以小驼峰下划线结束
    bool started_;
    bool joined_;
    pthread_t  pthreadId_;
    pid_t      tid_;
    ThreadFunc func_;
    string     name_;
    CountDownLatch latch_;//用于通知主线程 本线程初始化完毕

    static AtomicInt32 numCreated_;
    
};
}//namespace peak

#endif
