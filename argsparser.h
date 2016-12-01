#ifndef __ARGSPARSER_H__
#define __ARGSPARSER_H__

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <string>
#include <vector>

#include <stringutils.h>

typedef std::string string;

class ArgsParser {
private:
  int num_args;
  bool verbose;
  string args_info;
  std::vector<string> arg_vec;

  static const string SPACES;

private:
  int getArgPos(const string& arg) const;
  string formatHelp(const string& help) const;
  void showHelp(const string& arg, const string& type,
                const string& help,
                const string& default_val) const;

public:
  ArgsParser(const int args, char** argv)
      : num_args(args), verbose(false) {
    for (int i = 0; i < args; i++) arg_vec.push_back(argv[i]);
    if (args == 2 &&
        (arg_vec[1] == "-h" || arg_vec[1] == "--help"))
      verbose = true;
    if (verbose)
      fmt::print("usage: {} <key1> <val1> <key2> <val2> ...\n\n",
                 getCmd());
  }

  string getCmd() const;

  int getInt(const string& arg, const int default_val,
             const string& help = "") const;

  bool getBool(const string& arg, const bool default_val,
               const string& help = "") const;

  double getFlt(const string& arg, const double default_val,
                const string& help = "") const;

  string getStr(const string& arg, const string& default_val,
                const string& help = "") const;

  // don't worry about returning a     std::vector, as a move
  // operation will be
  // used under c++11.
  std::vector<int> getIntVec(const string& arg,
                             const std::vector<int>& default_val,
                             const string& help = "") const;

  std::vector<string> getStrVec(
      const string& arg, const std::vector<string>& default_val,
      const string& help = "") const;

  bool isEnd() const { return verbose; }
};

#endif /* __ARGSPARSER_H__ */
