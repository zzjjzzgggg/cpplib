/*
 * test_ioutils.cpp
 * Copyright (C) 2017 jzzhao <jzzhao@zhlap>
 *
 * Distributed under terms of the MIT license.
 */

#include <cstdio>
#include "../io/ioutils.h"
#include "../os/osutils.h"

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

void test_tsvparser() {
    printf("use file test.dat\n");
    ioutils::TSVParser ss("test.dat", ' ');
    ss.next();
    printf("%d\n", ss.getNumFields());
    while (ss.next()) printf("%d\t%d\n", ss.get<int>(0), ss.get<int>(1));
    printf("\n");

    ss.reset();
    while (ss.next()) printf("%d ", ss.get<int>(0));
    printf("\n");
}

int main(int argc, char* argv[]) {
    // test_null_ptr();
    test_tsvparser();
    return 0;
}
