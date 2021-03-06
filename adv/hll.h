/**
 * Copyright (C) by J.Z. (04/03/2018 17:46)
 * Distributed under terms of the MIT license.
 */

#ifndef __HYPER_LOGLOG_COUNTING_H__
#define __HYPER_LOGLOG_COUNTING_H__

#include <cstdint>
#include <cmath>

namespace hll {

double alpha(const int m);

double beta(const double ez);

/**
 * Count leading zeros within 8 bits
 */
uint8_t clz8(uint64_t x);

/**
 * x = max(x,y) in byte-wise, i.e., let x[i] denote the i-th byte of x, then
 * x[i] = max(x[i], y[i]), for i = 1,...
 */
void max(uint64_t& x, const uint64_t& y);

/**
 * Return true if x[i] >= y[i] for each byte i.
 */
bool isGreaterEqual(const uint64_t& x, const uint64_t& y);

/**
 * Given counter registers, use LC and HLL to estimate cardinality. If HLL
 * estimate < 10000, then use LC; otherwise use HLL.
 */
double count(const uint8_t* reg, const int m);

} /* namespace hll */

#endif /* __HYPER_LOGLOG_COUNTING_H__ */
