#ifndef __ARGSPARSER_H__
#define __ARGSPARSER_H__

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "../str/stringutils.h"

class ArgsParser {
private:
    int num_args;
    bool verbose;
    std::string args_info;
    std::vector<std::string> arg_vec;

    static const std::string SPACES;

private:
    int getArgPos(const std::string& arg) const;
    std::string formatHelp(const std::string& help) const;
    void showHelp(const std::string& arg, const std::string& type,
                  const std::string& help,
                  const std::string& default_val) const;

public:
    ArgsParser(const int args, char** argv) : num_args(args), verbose(false) {
        for (int i = 0; i < args; i++) arg_vec.push_back(argv[i]);
        if (args == 2 && (arg_vec[1] == "-h" || arg_vec[1] == "--help"))
            verbose = true;
        if (verbose)
            fmt::print("usage: {} <key1> <val1> <key2> <val2> ...\n\n",
                       getCmd());
    }

    std::string getCmd() const;

    int getInt(const std::string& arg, const int default_val,
               const std::string& help = "") const;

    bool getBool(const std::string& arg, const bool default_val,
                 const std::string& help = "") const;

    double getFlt(const std::string& arg, const double default_val,
                  const std::string& help = "") const;

    std::string getStr(const std::string& arg, const std::string& default_val,
                       const std::string& help = "") const;

    // don't worry about returning a std::vector, as a move
    // operation will be used under c++11.
    std::vector<int> getIntVec(const std::string& arg,
                               const std::vector<int>& default_val,
                               const std::string& help = "") const;

    std::vector<std::string> getStrVec(
        const std::string& arg, const std::vector<std::string>& default_val,
        const std::string& help = "") const;

    bool isEnd() const { return verbose; }
};

#endif /* __ARGSPARSER_H__ */
