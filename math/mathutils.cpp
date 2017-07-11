/**
 * Copyright (C) 2017 by J. Zhao
 * Distributed under terms of the MIT license.
 */

#include "mathutils.h"
#include <cmath>

double SpecFun::LogBinCoeff(const int k, const int n) {
    return std::lgamma(n + 1) - std::lgamma(k + 1) - std::lgamma(n - k + 1);
}

double SpecFun::BinCoeff(const int k, const int n) {
    return std::exp(LogBinCoeff(k, n));
}

double SpecFun::Binomial(const int k, const int n, const double p) {
    return std::exp(LogBinCoeff(k, n) + k * std::log(p) +
                    (n - k) * std::log(1 - p));
}

double SpecFun::BetaBinomial(const int k, const int n, const double alpha,
                             const double beta) {
    return std::exp(LogBinCoeff(k, n) + std::lgamma(k + alpha) +
                    std::lgamma(n - k + beta) - std::lgamma(n + alpha + beta) +
                    std::lgamma(alpha + beta) - std::lgamma(alpha) -
                    std::lgamma(beta));
}
