/**
 * Copyright (C) by J.Z. (07/27/2017 14:11)
 * Distributed under terms of the MIT license.
 */

#ifndef __RNGUTILS_H__
#define __RNGUTILS_H__

#include <unordered_set>

#include "random_generator.h"

namespace rngutils {

/**
 * Sample from a distribution.
 * Return: the index of the sampled element in the vector.
 * Require: sum(distribution) == 1
 */
template <typename RandomEngine = std::default_random_engine,
          typename DefaultSeedSeq = auto_seed_256>
const int sample(const std::vector<double> &dist,
                 random_generator<RandomEngine, DefaultSeedSeq> &rng) {
    int id = 0;
    double r = rng.uniform(), accu = dist[0];
    while (r > accu) accu += dist[++id];
    return id;
}

/**
 * Sample from a distribution.
 * Return: the index of the sampled element in the vector.
 * NOTE: This version does not need the weight vector is normalized, but need to
 * known the sum of weights.
 */
template <typename RandomEngine = std::default_random_engine,
          typename DefaultSeedSeq = auto_seed_256>
const int sample(const std::vector<double> &weights, const double sum_weights,
                 random_generator<RandomEngine, DefaultSeedSeq> &rng) {
    int id = 0;
    double r = rng.uniform() * sum_weights, accu = weights[0];
    while (r > accu) accu += weights[++id];
    return id;
}

/**
 * Choose an element from the population uniformly at random.
 * Return: the element in the population
 * Require: the population is non-empty
 */
template <typename Numeric, typename RandomEngine = std::default_random_engine,
          typename DefaultSeedSeq = auto_seed_256>
const Numeric choice(const std::vector<Numeric> &population,
                     random_generator<RandomEngine, DefaultSeedSeq> &rng) {
    size_t idx = rng.uniform(std::size_t{0}, population.size() - 1);
    return population[idx];
}

/**
 * Choose num elements from the population uniformly at random without
 * replacemet.
 *
 * Refer to https://stackoverflow.com/a/311716/1661545
 *
 * Return: a vector of samples, if population.size() < num, then return
 * population.
 * Require: the population is non-empty
 */
template <typename Numeric, typename RandomEngine = std::default_random_engine,
          typename DefaultSeedSeq = auto_seed_256>
std::vector<Numeric> choice(
    const std::vector<Numeric> &population, const int num,
    random_generator<RandomEngine, DefaultSeedSeq> &rng) {
    int N = population.size(), n = std::min(num, N);

    int t = 0;  // total input records dealt with
    int m = 0;  // number of items selected so far

    std::vector<Numeric> samples(n);

    while (m < n) {
        double u = rng.uniform();

        if ((N - t) * u >= n - m) {
            t++;
        } else {
            samples[m] = population[t];
            t++;
            m++;
        }
    }

    return samples;
}

/**
 * Initialize a vector 'dist' with uniformly distributed random numbers, s.t.
 * the vector sums to 1 from start index.
 */
void initUniform(std::vector<double> &dist, const size_t &start = 0);

}  // namespace rngutils

#endif /* __RNGUTILS_H__ */
