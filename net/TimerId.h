#ifndef HTTPSERVER_NET_TIMERID_H
#define HTTPSERVER_NET_TIMERID_H


namespace peak
{
namespace net
{

class Timer;

class TimerId
{ 
 public:
  TimerId() : timer_(NULL), sequence_(0)
  {
  }

  TimerId(Timer* timer, int64_t seq) : timer_(timer), sequence_(seq)
  {
  }


  friend class TimerQueue;

 private:
  Timer* timer_;
  int64_t sequence_;
};

}  // namespace net
}  // namespace peak

#endif
