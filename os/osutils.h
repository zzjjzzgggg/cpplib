#ifndef __OSUTILS_H__
#define __OSUTILS_H__

#include <sys/stat.h>
#include <cstdio>
#include <chrono>
#include <string>

#define FMT_HEADER_ONLY
#include "../fmt/fmt/format.h"

#include "../Assert/src/ppk_assert.h"
#define assert_msg PPK_ASSERT

namespace osutils {

inline bool fileExists(const std::string& name) {
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
}

inline char pathSeparator() {
#ifdef _WIN32
  return '\\';
#else
  return '/';
#endif
}

std::string joinPath(const std::string& parent,
                     const std::string& child);

class Timer {
private:
  std::chrono::steady_clock::time_point last_tick;

public:
  Timer() { tick(); }

  void tick() { last_tick = std::chrono::steady_clock::now(); }

  double milliseconds() const {
    auto&& dif = std::chrono::steady_clock::now() - last_tick;
    return std::chrono::duration<double, std::milli>(dif)
        .count();
  }

  double seconds() const { return milliseconds() / 1000; }

  const std::string getStr() const {
    return prettyTime(seconds());
  }

  static std::string prettyTime(const double secs) {
    if (secs < 60)
      return fmt::format("{:.2f}s", secs);
    else {
      int isecs = int(secs);
      if (isecs < 3600)
        return fmt::format("{:02d}m{:02d}s", isecs / 60,
                           isecs % 60);
      else
        return fmt::format("{:02d}h{:02d}m", isecs / 3600,
                           isecs % 3600 / 60);
    }
  }

  static char* curTime() {
    std::time_t t = std::time(NULL);
    static char buf[100];
    std::strftime(buf, sizeof(buf), "%F %T",
                  std::localtime(&t));
    return buf;
  }

  static int timestamp() {
    auto dur =
        std::chrono::steady_clock::now().time_since_epoch();
    return std::chrono::duration_cast<
               std::chrono::milliseconds>(dur)
        .count();
  }
};
}

#endif /* __OSUTILS_H__ */
