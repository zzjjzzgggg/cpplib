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
  return 0;
}
