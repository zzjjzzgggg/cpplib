#include <cstdio>

#include "argsparser.h"

int main(int argc, char *argv[]) {
  ArgsParser parser(argc, argv);
  parser.getInt("-i", 0,
                        "give me an integer\n"
                        "this is a long help msg\n"
                        "seperated by '\\n'");
  parser.isEnd();
  return 0;
}
