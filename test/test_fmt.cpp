#include <cstdio>

#include <tuple>

#define FMT_HEADER_ONLY
#include "../fmt/fmt/format.h"

void test_fmt_tuple() {
    auto t = std::make_tuple(1, 2, 3.4);
    auto s = std::apply(
        [=](const auto &... args) {
            return fmt::format("{}\t{}\t{}\n", args...);
        },
        t);
    printf("%s\n", s.c_str());

    auto e = std::apply(
        [=](const auto &... args) { return std::make_tuple(args...); }, t);

    printf("%f\n", std::get<2>(e));
}

int main(int argc, char *argv[]) {
    // printf("%s", fmt::format("hhh {}\n", 1).c_str());
    test_fmt_tuple();

    return 0;
}
