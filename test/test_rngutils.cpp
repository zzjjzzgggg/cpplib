#include <cstdio>
#include "../adv/rngutils.h"

int main(int argc, char *argv[]) {
    rngutils::default_rng rng;
    std::vector<int> population_int = {1, 2, 3, 4, 5};
    int s_int = rngutils::choice(population_int, rng);
    printf("%d\n", s_int);

    std::vector<double> population_double = {1, 2, 3, 4, 5};
    double s_double = rngutils::choice(population_double, rng);
    printf("%.2f\n", s_double);

    std::vector<double> pop(10);
    rngutils::initUniform(pop, 2);
    for (auto val : pop) printf("%.2f  ", val);
    printf("\n");

    double sum = std::accumulate(pop.begin(), pop.end(), 0.0);
    printf("%.2f\n", sum);
    return 0;
}
