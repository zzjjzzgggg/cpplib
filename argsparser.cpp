#include "argsparser.h"

const string ArgsParser::SPACES(24, ' ');

string ArgsParser::formatHelp(const string& help) const {
  if (help.find('\n') == std::string::npos) return help;
  std::vector<string> elems = strutils::split(help, '\n');
  fmt::MemoryWriter w;
  w.write("{}", elems[0]);
  for (size_t i = 1; i < elems.size(); i++) {
    w.write("\n                        {}", elems[i]);
  }
  return w.str();
}

void ArgsParser::showHelp(const string& arg, const string& type,
                          const string& help,
                          const string& default_val) const {
  if (verbose)
    fmt::print("{:4}{:<12}{:<8}{} (default: {})\n", " ", arg,
               type, formatHelp(help), default_val);
}

string ArgsParser::getCmd() const {
  string cmd = arg_vec[0];
  size_t pos = cmd.find_last_of('/');
  if (pos == string::npos)
    return cmd;
  else
    return cmd.substr(pos + 1);
}

int ArgsParser::getArgPos(const string& arg) const {
  int arg_pos = 0;
  for (auto&& a : arg_vec) {
    if (a == arg) return arg_pos;
    arg_pos++;
  }
  return -1;
}

bool ArgsParser::getBool(const string& arg,
                         const bool default_val,
                         const string& help) const {
  showHelp(arg, "[bool]", help, default_val ? "true" : "false");
  int arg_pos = getArgPos(arg);
  if (arg_pos < 0 || num_args < arg_pos + 2)
    return default_val;
  else
    return arg_vec[arg_pos + 1] == "true";
}

int ArgsParser::getInt(const string& arg, const int default_val,
                       const string& help) const {
  showHelp(arg, "[int]", help, std::to_string(default_val));
  int rst;
  int arg_pos = getArgPos(arg);
  if (arg_pos < 0 || num_args < arg_pos + 2)
    rst = default_val;
  else
    rst = atoi(arg_vec[arg_pos + 1].c_str());
  return rst;
}

double ArgsParser::getFlt(const string& arg,
                          const double default_val,
                          const string& help) const {
  showHelp(arg, "[flt]", help, std::to_string(default_val));
  double rst;
  int arg_pos = getArgPos(arg);
  if (arg_pos < 0 || num_args < arg_pos + 2)
    rst = default_val;
  else
    rst = atof(arg_vec[arg_pos + 1].c_str());
  return rst;
}

string ArgsParser::getStr(const string& arg,
                          const string& default_val,
                          const string& help) const {
  showHelp(arg, "[str]", help, default_val);
  int arg_pos = getArgPos(arg);
  if (arg_pos < 0 || num_args < arg_pos + 2) return default_val;
  return arg_vec[arg_pos + 1];
}

std::vector<int> ArgsParser::getIntVec(
    const string& arg, const std::vector<int>& default_val,
    const string& help) const {
  std::vector<int> arg_ints;
  return arg_ints;
}

std::vector<string> ArgsParser::getStrVec(
    const string& arg, const std::vector<string>& default_val,
    const string& help) const {
  std::vector<string> arg_strs;
  return arg_strs;
}
