#include "timer.hpp"

Timer::Timer() {}
Timer::~Timer() {}

void Timer::start() {
  startedTime = clock();
}

int Timer::getTime() {
  return ((clock() - startedTime) / (CLOCKS_PER_SEC / 1000));
}
