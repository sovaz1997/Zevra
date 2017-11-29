#ifndef TIMER_HPP
#define TIMER_HPP

#include <cstdint>
#include <ctime>

class Timer {
private:
  int64_t startedTime;
public:
  Timer();
  ~Timer();
  void start();
  int64_t getTime();
};

#endif
