/*
 * test_osutils.cpp
 * Copyright (C) 2017 jzzhao <jzzhao@zhlap>
 *
 * Distributed under terms of the MIT license.
 */

#include <cstdio>
#include "osutils.h"

int main(int argc, char *argv[]) {
  std::string prefix = osutils::join("../../syngraphs/", "./");
  printf("%s\n", prefix.c_str());
  printf("%d\n", osutils::exists("/home/jzzhao"));
  printf("%s\n", osutils::join("/home/jzzhao", "greedy").c_str());
  osutils::rmfile("/home/jzzhao/workspace/test/test");
  return 0;
}
