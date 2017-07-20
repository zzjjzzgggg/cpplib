/*
 * test_ioutils.cpp
 * Copyright (C) 2017 jzzhao <jzzhao@zhlap>
 *
 * Distributed under terms of the MIT license.
 */

#include <cstdio>
#include "ioutils.h"
#include "osutils.h"

using namespace ioutils;
using namespace std;

int main(int argc, char* argv[]) {
    string src_file = "~/workspace/discoverability/hepth/graph.gz";
    string dst_file = "test.lz";
    auto po = getIOOut(dst_file);

    printf("writing ... \n");
    TSVParser parser(src_file);
    while (parser.next()) {
        int x = parser.get<int>(0);
        int y = parser.get<int>(1);
        po->save(fmt::format("{}\t{}\n", x, y));
    }
    po->close();

    auto edges = loadPrVec<int, int>(src_file);

    printf("reading ... \n");
    int i = 0;
    TSVParser ss(dst_file);
    while (ss.next()) {
        int x = ss.get<int>(0);
        int y = ss.get<int>(1);
        if (x != edges[i].first || y != edges[i].second) {
            printf("err\n");
            break;
        }
        i++;
    }

    return 0;
}
