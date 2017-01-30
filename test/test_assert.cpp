#include <cstdio>

#include "ppk_assert.h"
#define assert_msg PPK_ASSERT

int main(int argc, char *argv[]) {
  int a = 1, b = 2;
  assert_msg(a == b, "%d not equal to %d\n", a, b);
  return 0;
}
