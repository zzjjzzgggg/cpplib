/**
 * Copyright (C) 2017 by J. Zhao
 * Distributed under terms of the MIT license.
 */

#ifndef __MATHUTILS_H__
#define __MATHUTILS_H__

#include <cmath>

class SpecFun {
public:
    /**
     * log of Binomial coefficient.
     */
    inline static double LogBinCoeff(const int k, const int n);

    /**
     * Binomial coefficient, i.e., n choose k.
     *
     * B(k, n) =
     *            Gamma(n + 1)
     *   ------------------------------
     *    Gamma(k + 1)Gamma(n - k + 1)
     */
    static double BinCoeff(const int k, const int n);

    /**
     * Binomial distribution.
     *
     * Bin(k | n, p) = B(k, n) p^k (1-p)^{n-k}
     *
     */
    static double Binomial(const int k, const int n, const double p);

    /**
     * Beta-Binomial distribution.
     *
     * f(k | n, alpha, beta) =
     *
     *    Gamma(alpha + beta)                 Gamma(n + 1)
     *   ------------------------ x ------------------------------ x
     *   Gamma(alpha)Gamma(beta)     Gamma(k + 1)Gamma(n - k + 1)
     *
     *    Gamma(k + alpah)Gamma(n - k + beta)
     *   -------------------------------------
     *         Gamma(n + alpha + beta)
     */
    static double BetaBinomial(const int k, const int n, const double alpha,
                               const double beta);
};

#endif /* __MATHUTILS_H__ */
