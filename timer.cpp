#include "timer.hpp"

Timer::Timer() {}
Timer::~Timer() {}

void Timer::start() {
  startedTime = std::chrono::system_clock::now();//clock();
}

int64_t Timer::getTime() {
  //return ((clock() - startedTime) / (CLOCKS_PER_SEC / 1000));
  return std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now() - startedTime).count();
}