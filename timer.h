#ifndef __TIMER_H__
#define __TIMER_H__

#include <cstdio>
#include <chrono>

#include <string>
#include <stringutils.h>

class Timer {
private:
  std::chrono::steady_clock::time_point last_tick;

public:
  Timer() { tick(); }

  void tick() { last_tick = std::chrono::steady_clock::now(); }

  double milliseconds() const {
    auto&& dif = std::chrono::steady_clock::now() - last_tick;
    return std::chrono::duration<double, std::milli>(dif).count();
  }

  double seconds() const { return milliseconds() / 1000; }

  const std::string getStr() const {
    return strutils::prettyTime(seconds());
  }

  static char* curTime() {
    std::time_t t = std::time(NULL);
    static char buf[100];
    std::strftime(buf, sizeof(buf), "%F %T", std::localtime(&t));
    return buf;
  }

  static int timestamp() {
    auto dur =
        std::chrono::steady_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               dur)
        .count();
  }
};

#endif /* __TIMER_H__ */
