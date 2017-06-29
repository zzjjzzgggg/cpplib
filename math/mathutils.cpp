/**
 * Copyright (C) 2017 by J. Zhao
 * Distributed under terms of the MIT license.
 */

#include "mathutils.h"
#include <cmath>

/**
 * f(k|n,alpha,beta) =
 *    T(alpha + beta)      T(n + 1)      T(k+alpah)T(n-k+beta)
 *   -----------------.----------------.-----------------------
 *    T(alpha)T(beta)   T(k+1)T(n-k+1)      T(n+alpha+beta)
 **/
double SpecFunc::BetaBinomial(const int k, const int n, const double alpha,
                              const double beta) {
    return exp(lgamma(n + 1.0) - lgamma(k + 1.0) - lgamma(n - k + 1.0) +
               lgamma(k + alpha) + lgamma(n - k + beta) -
               lgamma(n + alpha + beta) + lgamma(alpha + beta) - lgamma(alpha) -
               lgamma(beta));
}
