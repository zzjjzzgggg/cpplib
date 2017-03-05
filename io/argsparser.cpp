#include "argsparser.h"

const std::string ArgsParser::SPACES(24, ' ');

std::string ArgsParser::formatHelp(
    const std::string& help) const {
  if (help.find('\n') == std::string::npos) return help;
  std::vector<std::string> elems =
      strutils::split(help, '\n');
  fmt::MemoryWriter w;
  w.write("{}", elems[0]);
  for (size_t i = 1; i < elems.size(); i++) {
    w.write("\n                        {}", elems[i]);
  }
  return w.str();
}

void ArgsParser::showHelp(
    const std::string& arg, const std::string& type,
    const std::string& help,
    const std::string& default_val) const {
  if (verbose)
    fmt::print("{:4}{:<12}{:<8}{} (default: {})\n", " ", arg,
               type, formatHelp(help), default_val);
}

std::string ArgsParser::getCmd() const {
  std::string cmd = arg_vec[0];
  size_t pos = cmd.find_last_of('/');
  if (pos == std::string::npos)
    return cmd;
  else
    return cmd.substr(pos + 1);
}

int ArgsParser::getArgPos(const std::string& arg) const {
  int arg_pos = 0;
  for (auto&& a : arg_vec) {
    if (a == arg) return arg_pos;
    arg_pos++;
  }
  return -1;
}

bool ArgsParser::getBool(const std::string& arg,
                         const bool default_val,
                         const std::string& help) const {
  showHelp(arg, "[bool]", help,
           default_val ? "true" : "false");
  int arg_pos = getArgPos(arg);
  if (arg_pos < 0 || num_args < arg_pos + 2)
    return default_val;
  else
    return arg_vec[arg_pos + 1] == "true";
}

int ArgsParser::getInt(const std::string& arg,
                       const int default_val,
                       const std::string& help) const {
  showHelp(arg, "[int]", help, std::to_string(default_val));
  int rst;
  int arg_pos = getArgPos(arg);
  if (arg_pos < 0 || num_args < arg_pos + 2)
    rst = default_val;
  else
    rst = atoi(arg_vec[arg_pos + 1].c_str());
  return rst;
}

double ArgsParser::getFlt(const std::string& arg,
                          const double default_val,
                          const std::string& help) const {
  showHelp(arg, "[flt]", help, std::to_string(default_val));
  double rst;
  int arg_pos = getArgPos(arg);
  if (arg_pos < 0 || num_args < arg_pos + 2)
    rst = default_val;
  else
    rst = atof(arg_vec[arg_pos + 1].c_str());
  return rst;
}

std::string ArgsParser::getStr(const std::string& arg,
                               const std::string& default_val,
                               const std::string& help) const {
  showHelp(arg, "[str]", help, default_val);
  int arg_pos = getArgPos(arg);
  if (arg_pos < 0 || num_args < arg_pos + 2)
    return default_val;
  return arg_vec[arg_pos + 1];
}

std::vector<int> ArgsParser::getIntVec(
    const std::string& arg,
    const std::vector<int>& default_val,
    const std::string& help) const {
  std::vector<int> arg_ints;
  return arg_ints;
}

std::vector<std::string> ArgsParser::getStrVec(
    const std::string& arg,
    const std::vector<std::string>& default_val,
    const std::string& help) const {
  std::vector<std::string> arg_strs;
  return arg_strs;
}
