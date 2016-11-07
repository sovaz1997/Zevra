#ifndef TIMER_HPP
#define TIMER_HPP

#include <ctime>

class Timer {
private:
  int startedTime;
public:
  Timer();
  ~Timer();
  void start();
  int getTime();
};

#endif
