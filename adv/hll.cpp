/**
 * Copyright (C) by J.Z. (04/03/2018 17:55)
 * Distributed under terms of the MIT license.
 */

#include "hll.h"

namespace hll {

double alpha(const int m) {
    switch (m) {
        case 16:
            return 0.673;
        case 32:
            return 0.697;
        case 64:
            return 0.709;
        default:
            return 0.7213 / (1 + 1.079 / m);
    }
}

double beta(const double ez) {
    double zl = std::log(ez + 1);
    return -0.37331876643753059 * ez + -1.41704077448122989 * zl +
           0.40729184796612533 * std::pow(zl, 2) +
           1.56152033906584164 * std::pow(zl, 3) +
           -0.99242233534286128 * std::pow(zl, 4) +
           0.26064681399483092 * std::pow(zl, 5) +
           -0.03053811369682807 * std::pow(zl, 6) +
           0.00155770210179105 * std::pow(zl, 7);
}

/**
 * Count leading zeros within 8 bits
 */
uint8_t clz8(uint64_t x) {
    if ((x & 0xFF00000000000000) == 0) return 8;
    uint8_t n;
    if ((x & 0xF000000000000000) == 0) {
        n = 4;
        x <<= 4;
    } else
        n = 0;
    return n + CLZ_TABLE_4BIT[(x >> 60) & 0x0F];
}

void max(uint64_t& x, const uint64_t& y) {
    uint64_t z = ((((x | H8) - (y & ~H8)) | (x ^ y)) ^ (x | ~y)) & H8,
             m = ((((z >> 7) | H8) - L8) | H8) ^ z;
    x = (x & m) | (y & ~m);
}

bool isGreaterEqual(const uint64_t& x, const uint64_t& y) {
    uint64_t z = ((((x | H8) - (y & ~H8)) | (x ^ y)) ^ (x | ~y)) & H8,
        m = ((((z >> 7) | H8) - L8) | H8) ^ z;
    return m == A1;
}

double count(const uint8_t* reg, const int m) {
    double sum = 0, ez = 0;
    for (int i = 0; i < m; i++) {
        sum += 1.0 / (1 << reg[i]);
        if (reg[i] == 0) ez++;
    }
    sum += beta(ez);
    double est_HLL = alpha(m) * m * (m - ez) / sum + 0.5,
           est_LC = m * std::log(m / ez);
    return est_HLL < 10000 ? est_LC : est_HLL;
}

} /* namespace hll */
