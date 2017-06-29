/**
 * Copyright (C) 2017 by J. Zhao
 * Distributed under terms of the MIT license.
 */

#include <cstdio>
#include "../math/mathutils.h"

int main(int argc, char *argv[]) {
    double s = 0;
    for (int i = 0; i <= 3; i++) {
        double val = SpecFunc::BetaBinomial(i, 3, .5, .5);
        s += val;
        printf("%.3f\n", val);
    }
    printf("\n%.3f\n", s);
    return 0;
}
