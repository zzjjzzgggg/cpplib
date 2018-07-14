/**
 * Copyright (C) by J.Z. (2018-07-12 10:46)
 * Distributed under terms of the MIT license.
 */

#include <cstdint>
#include <cmath>

#include <iostream>
#include <bitset>

#include "../adv/hll.h"

#include <gflags/gflags.h>

DEFINE_int32(x, 10, "x");
DEFINE_int32(y, 10, "y");

constexpr uint64_t L8 = 0x0101010101010101;
constexpr uint64_t H8 = 0x8080808080808080;
constexpr uint64_t A1 = 0xffffffffffffffff;

void merge() {
    uint64_t x = FLAGS_x, y = FLAGS_y;

    std::cout << "x: " << std::bitset<32>(x) << std::endl;
    std::cout << "y: " << std::bitset<32>(y) << std::endl;



    std::cout << "x >= y: " << hll::isGreaterEqual(x, y) << std::endl;

    hll::max(x, y);
    std::cout << "max(x, y): " << std::bitset<32>(x) << std::endl;
}

int main(int argc, char *argv[]) {
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    merge();

    gflags::ShutDownCommandLineFlags();
    return 0;
}
