/*
 * test_osutils.cpp
 * Copyright (C) 2017 jzzhao <jzzhao@zhlap>
 *
 * Distributed under terms of the MIT license.
 */

#include <cstdio>
#include "osutils.h"

int main(int argc, char *argv[]) {
  printf("%d\n", osutils::fileExists("/home/jzzhao"));
  printf("%s\n", osutils::joinPath("/home/jzzhao", "greedy").c_str());
  osutils::rmfile("/home/jzzhao/workspace/test/test");
  return 0;
}
