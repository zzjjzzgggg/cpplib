#include <cstdio>

#include "strutils.h"

int main(int argc, char *argv[]) {
    printf("%s\n", strutils::prettySize(2048).c_str());
    return 0;
}
