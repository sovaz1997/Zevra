#ifndef TIMER_HPP
#define TIMER_HPP

#include <cstdint>
#include <ctime>
#include <chrono>

class Timer {
private:
  std::chrono::time_point<std::chrono::system_clock> startedTime;
public:
  Timer();
  ~Timer();
  void start();
  int64_t getTime();
};

#endif
