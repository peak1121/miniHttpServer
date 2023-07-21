#ifndef HTTPSERVER_NET_TIMER_H
#define HTTPSERVER_NET_TIMER_H

#include "base/Atomic.h"
#include "base/Timestamp.h"
#include "Callbacks.h"

namespace peak
{
namespace net
{

///
/// Internal class for timer event.
///
/*

*/
class Timer
{
 public:
  Timer(TimerCallback cb, Timestamp when, double interval)
    : callback_(std::move(cb)),
      expiration_(when),
      interval_(interval),
      repeat_(interval > 0.0),
      sequence_(s_numCreated_.incrementAndGet())
  { }

  void run() const
  {
    callback_();
  }

  Timestamp expiration() const  { return expiration_; }
  bool repeat() const { return repeat_; }
  int64_t sequence() const { return sequence_; }

  void restart(Timestamp now);

  static int64_t numCreated() { return s_numCreated_.get(); }

 private:
  const TimerCallback callback_;//定时器到期的回调函数
  Timestamp expiration_;//定时器发生的时间
  const double interval_;//定时器间隔多久重复发送，如果大于0的话说明是重复定时器
  const bool repeat_;//间隔大于0为true
  const int64_t sequence_;//当前定时器序号

  static AtomicInt64 s_numCreated_;//记录全局定时器数量
};

}  // namespace net
}  // namespace peak

#endif  // 
