/**
 * Copyright (C) by J.Z. (07/27/2017 14:16)
 * Distributed under terms of the MIT license.
 */

#include "rngutils.h"

namespace rngutils {

void initUniform(std::vector<double> &dist, const size_t &start) {
    if (start > 0) std::fill(dist.begin(), dist.begin() + start, 0);
    double sum = 0;
    rngutils::default_rng rng;
    for (size_t i = start; i < dist.size(); i++) {
        dist[i] = rng.uniform();
        sum += dist[i];
    }
    for (size_t i = start; i < dist.size(); i++) dist[i] /= sum;
}

}  // namespace rngutils
