#include <cstdio>
#include <climits>
#include <bitset>
#include <iostream>
#include <map>

#include "../adv/rngutils.h"
#include "../io/ioutils.h"

using namespace rngutils;

void test_basic() {
    rngutils::default_rng rng;

    printf("\n choose 1:\n");
    std::vector<int> population = {1, 2, 3, 4, 5};
    int s_int = rngutils::choice(population, rng);
    printf("%d\n", s_int);

    printf("\n choose 3:\n");
    auto samples = rngutils::choice(population, 3, rng);
    for (int s : samples) printf(" %d,", s);
    printf("\n");

    printf("\n init uniform:\n");
    std::vector<double> pop(10);
    rngutils::initUniform(pop, 2);
    for (auto val : pop) printf("%.2f  ", val);
    printf("\n");
    double sum = std::accumulate(pop.begin(), pop.end(), 0.0);
    printf("sum: %.2f\n", sum);
}

void test_rng() {
    rngutils::default_rng rng;
    for (int i = 0; i < 10; i++) {
        auto r = rng.randint<unsigned int>();
        std::cout << std::bitset<32>(r) << "  " << r << std::endl;
    }
    std::cout << std::bitset<32>(UINT_MAX) << "  " << UINT_MAX << std::endl;
}

void test_geo() {
    rngutils::default_rng rng;
    std::map<int, int> hist;
    for (int n = 0; n < 10000; ++n) {
        hist[rng.geometric(0.5)]++;
    }

    for (auto p : hist) {
        std::cout << p.first << ' ' << std::string(p.second / 100, '*') << '\n';
    }
}

void test_choice() {
    rngutils::default_rng rng;
    std::vector<int> pop = {0, 1, 2};
    auto vec = rngutils::choice(pop, 2, rng);
    ioutils::printVec(vec);
}

int main(int argc, char *argv[]) {
    // test_basic();
    // test_geo();
    test_choice();

    return 0;
}
