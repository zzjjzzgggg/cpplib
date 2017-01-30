/*
 * test_ioutils.cpp
 * Copyright (C) 2017 jzzhao <jzzhao@zhlap>
 *
 * Distributed under terms of the MIT license.
 */

#include <cstdio>
#include "ioutils.h"

using namespace ioutils;

int main(int argc, char *argv[]) {
  LZ4Out lzo("/home/jzzhao/workspace/test/lz4/oneblock.lz4");
  const char* data = "this is a test aaa";
  printf("%lu, %lu\n", std::strlen(data), strlen(data));
  lzo.write(data, std::strlen(data));
  return 0;
}
