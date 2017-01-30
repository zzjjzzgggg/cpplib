#include <cstdio>

#define FMT_HEADER_ONLY
#include "../fmt/fmt/format.h"

int main(int argc, char *argv[]) {
  printf("%s", fmt::format("hhh {}\n", 1).c_str());
  return 0;
}
