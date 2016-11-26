#ifndef __TIMER_H__
#define __TIMER_H__

#include <cstdio>
#include <chrono>

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

  const char* str() const {
    double secs = seconds();
    static char buf[32];
    if (secs < 60)
      sprintf(buf, "%.2fs", secs);
    else {
      int isecs = int(secs);
      if (isecs < 3600)
        sprintf(buf, "%02dm%02ds", isecs / 60, isecs % 60);
      else
        sprintf(buf, "%02dh%02dm", isecs / 3600,
                isecs % 3600 / 60);
    }
    return buf;
  }

  static char* curtime() {
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
