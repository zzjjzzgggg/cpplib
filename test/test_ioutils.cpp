/*
 * test_ioutils.cpp
 * Copyright (C) 2017 jzzhao <jzzhao@zhlap>
 *
 * Distributed under terms of the MIT license.
 */

#include <cstdio>
#include "ioutils.h"
#include "osutils.h"

int main(int argc, char* argv[]) {
    ioutils::printVec<int>({1, 2, 3, 4});
    return 0;
}
