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

double SpecFun::LogBetaCoeff(const double alpha, const double beta) {
    return std::lgamma(alpha) + std::lgamma(beta) - std::lgamma(alpha + beta);
}

double SpecFun::BetaCoeff(const double alpha, const double beta) {
    return std::exp(LogBetaCoeff(alpha, beta));
}

double SpecFun::Binomial(const int k, const int n, const double p) {
    return std::exp(LogBinCoeff(k, n) + k * std::log(p) +
                    (n - k) * std::log(1 - p));
}

double SpecFun::BetaBinomial(const int k, const int n, const double alpha,
                             const double beta) {
    return std::exp(LogBinCoeff(k, n) + LogBetaCoeff(alpha + k, beta + n - k) -
                    LogBetaCoeff(alpha, beta));
}

double SpecFun::BetaGeometric(const int k, const double alpha,
                              const double beta) {
    return std::exp(LogBetaCoeff(alpha + 1, beta + k - 1) -
                    LogBetaCoeff(alpha, beta));
}
