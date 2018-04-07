#include <cstdio>

#include "strutils.h"

using namespace fmt::literals;

int main(int argc, char *argv[]) {
    printf("%s\n", strutils::prettySize(2048).c_str());
    std::string s = "a={:g}\n"_format(0.1);
    printf("%s\n", s.c_str());

    return 0;
}
