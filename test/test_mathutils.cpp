/**
 * Copyright (C) 2017 by J. Zhao
 * Distributed under terms of the MIT license.
 */

#include <cstdio>
#include "../math/mathutils.h"

int main(int argc, char *argv[]) {
    int n = 0;
    double sum = 0;
    for (int i = 0; i <= n; i++) {
        double val = SpecFunc::BetaBinomial(i, n, .5, .5);
        sum += val;
        printf("%.3f\n", val);
    }
    printf("\n%.3f\n", sum);
    return 0;
}
