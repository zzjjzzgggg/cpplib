/**
 * Copyright (C) 2017 by J. Zhao
 * Distributed under terms of the MIT license.
 */

#include <cstdio>
#include "../math/mathutils.h"

double pr(int x, double mu, double theta) {
    double val = mu / (1 + (x - 1) * theta);
    for (int i = 1; i < x; i++)
        val *= (1 - mu + (i - 1) * theta) / (1 + (i - 1) * theta);
    return val;
}

int main(int argc, char *argv[]) {
    int n = 10;
    double mu = .2, theta = .01, alpha = mu / theta, beta = (1 - mu) / theta,
           sum = 0, sum2 = SpecFun::BetaBinomial(0, n, alpha, beta);

    for (int i = 1; i <= n; i++) {
        double val = SpecFun::BetaGeometric(i, alpha, beta);
        printf("%d: %.6f, %.6f\n", i, val, pr(i, mu, theta));
        sum += val;
        sum2 += pr(i, mu, theta);
    }
    printf("\n%.4f, %.4f\n", sum, sum2);

    return 0;
}
