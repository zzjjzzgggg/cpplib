#include <cstdio>

#include "stringutils.h"

int main(int argc, char *argv[]) {
  printf("%s\n", strutils::prettySize(2048).c_str());
  return 0;
}
