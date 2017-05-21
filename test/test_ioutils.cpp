/*
 * test_ioutils.cpp
 * Copyright (C) 2017 jzzhao <jzzhao@zhlap>
 *
 * Distributed under terms of the MIT license.
 */

#include <cstdio>
#include "ioutils.h"

using namespace ioutils;

int main(int argc, char* argv[]) {
    TSVParser parser("test.dat");
    if (parser.next()) {
        int x = parser.get<int>(0);
        int y = parser.get<int>(1);
        printf("%d\t%d\n", x, y);
    }
    if (parser.next()) {
        double x = parser.get<double>(0);
        double y = parser.get<double>(1);
        printf("%.2e\t%.2e\n", x, y);
    }
    return 0;
}
