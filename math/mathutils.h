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
    inline static double LogBinCoeff(const int k, const int n) {
        return std::lgamma(n + 1) - std::lgamma(k + 1) - std::lgamma(n - k + 1);
    }

    /**
     * Binomial coefficient, i.e., n choose k.
     *
     *                        Gamma(n + 1)
     * BinCoef(k, n) = ------------------------------
     *                  Gamma(k + 1)Gamma(n - k + 1)
     */
    static double BinCoeff(const int k, const int n);

    /**
     * log of Beta distribution coefficient
     */
    inline static double LogBetaCoeff(const double alpha, const double beta) {
        return std::lgamma(alpha) + std::lgamma(beta) -
               std::lgamma(alpha + beta);
    }

    /**
     * Beta distribution coefficient.
     *
     *                          Gamma(alpha)Gamma(beta)
     * BetaCoef(alpha, beta) = -------------------------
     *                            Gamma(alpha + beta)
     */
    static double BetaCoeff(const double alpha, const double beta);

    /**
      * Binomial distribution.
      *
      * Bin(k | n, p) = BinCoef(k, n) p^k (1-p)^{n-k}
      *
      */
    static double Binomial(const int k, const int n, const double p);

    /**
     * Beta-Binomial distribution. BetaBin(k; n, alpha, beta), k \in [0,n]
     *
     * f(k | n, alpha, beta) = BinCoef(k, n) x
     *
     *    BetaCoef(alpha + k, beta + n - k)
     *   -----------------------------------
     *          BetaCoef(alpha, beta)
     *
     */
    static double BetaBinomial(const int k, const int n, const double alpha,
                               const double beta);

    /**
     * Beta-Geometric distribution. BetaGeo(k; alpha, beta), k \in [1,\infty)
     *
     *                       BetaCoef(alpha + 1, beta + k - 1)
     * f(k | alpha, beta) = -------------------------------
     *                           BetaCoef(alpha, beta)
     *    alpha x Gamma(alpha + beta) x Gamma(k + beta)
     *   -----------------------------------------------
     *      Gamma(beta) x Gamma(alpha + beta + k + 1)
     */
    static double BetaGeometric(const int k, const double alpha,
                                const double beta);
};

#endif /* __MATHUTILS_H__ */
