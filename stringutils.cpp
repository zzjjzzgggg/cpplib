#include "stringutils.h"

namespace strutils {
void split_filename(const std::string &fullname,
                    std::string &base,
                    std::string &filename_wo_ext,
                    std::string &ext) {
  // base.clear();
  // filename_wo_ext.clear();
  // ext.clear();
  if (fullname.empty()) return;
  auto dir_idx = fullname.rfind('/');
  std::string filename_w_ext;
  if (dir_idx != std::string::npos) {
    base = fullname.substr(0, dir_idx + 1);
    filename_w_ext = fullname.substr(dir_idx + 1);
  } else {
    base = "./";
    filename_w_ext = fullname;
  }
  auto ext_idx = filename_w_ext.rfind('.');
  filename_wo_ext = filename_w_ext.substr(0, ext_idx);
  if (ext_idx != std::string::npos)
    ext = filename_w_ext.substr(ext_idx);
}

/**
 * ../syn/events2_U100_I10_T100.dat -->
 * ../syn/events2_U100_I10_T100_test.dat
 */
std::string insert_middle(const std::string &filename,
                          const std::string &sufix) {
  auto idx = filename.rfind('.');
  if (idx != std::string::npos) {
    std::string extension = filename.substr(idx + 1);
    std::string base = filename.substr(0, idx);
    return base + "_" + sufix + "." + extension;
  } else
    return filename + "_" + sufix;
}

std::string get_base_path(const std::string &fullname) {
  auto idx = fullname.rfind('/');
  if (idx != std::string::npos) {
    return fullname.substr(0, idx + 1);
  } else {
    return "./";
  }
}

std::string pretty_number(const int num) {
  if (num < 1e3)
    return fmt::format("{}", num);
  else if (num < 1e6)
    return fmt::format("{}K", num / 1000);
  else if (num < 1e9)
    return fmt::format("{}M", num / 1000000);
  else
    return fmt::format("{}G", num / 1000000000);
}

std::string pretty_size(const int size) {
  if (size < Kilobytes)
    return fmt::format("{}B", size);
  else if (size < Megabytes)
    return fmt::format("{:.2f}K", size / Kilobytes);
  else if (size < Gigabytes)
    return fmt::format("{:.2f}M", size / Megabytes);
  else
    return fmt::format("{:.2f}G", size / Gigabytes);
}

std::string pretty_time(const double secs) {
  if (secs < 60)
    return fmt::format("{:.2f}s", secs);
  else {
    int isecs = int(secs);
    if (isecs < 3600)
      return fmt::format("{:02d}m{:02d}s", isecs / 60, isecs % 60);
    else
      return fmt::format("{:02d}h{:02d}m", isecs / 3600,
                        isecs % 3600 / 60);
  }
}
}
