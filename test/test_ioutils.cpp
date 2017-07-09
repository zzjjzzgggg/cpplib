/*
 * test_ioutils.cpp
 * Copyright (C) 2017 jzzhao <jzzhao@zhlap>
 *
 * Distributed under terms of the MIT license.
 */

#include <cstdio>
#include "ioutils.h"

using namespace ioutils;
using namespace std;

int main(int argc, char* argv[]) {
    int n = 0;
    GZipOut zo("test.gz");
    printf("writing ... \n");
    TSVParser parser("~/workspace/discoverability/enron/graph.gz");
    while (parser.next()) {
        n++;
        string x = parser.get<string>(0);
        string y = parser.get<string>(1);
        printf("%s\t%s\n", x.c_str(), y.c_str());
        zo.save(fmt::format("{}\t{}\n", x.c_str(), y.c_str()));
        if (n > 10) break;
    }
    zo.close();

    printf("reading ... \n");
    TSVParser ss("test.gz");
    while (ss.next()) {
        string x = ss.get<string>(0);
        string y = ss.get<string>(1);
        printf("%s\t%s\n", x.c_str(), y.c_str());
    }

    return 0;
}
