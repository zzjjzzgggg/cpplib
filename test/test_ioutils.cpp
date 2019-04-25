/*
 * test_ioutils.cpp
 * Copyright (C) 2017 jzzhao <jzzhao@zhlap>
 *
 * Distributed under terms of the MIT license.
 */

#include <cstdio>
#include "ioutils.h"
#include "osutils.h"

void test_null_ptr() {
    // auto ptr = std::unique_ptr<ioutils::IOIn>();
    auto ptr = ioutils::getIOIn("dad");
    if (ptr)
        printf("not null\n");
    else
        printf("null ptr\n");
}

void test_print() {
    ioutils::printVec<int>({1, 2, 3, 4});

    std::unordered_map<int, int> m;
    m[1] = 2;
    m[2] = 3;
    ioutils::printMap<int, int>(m);
}

int main(int argc, char* argv[]) {
    test_null_ptr();
    return 0;
}
