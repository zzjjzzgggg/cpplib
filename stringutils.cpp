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
}
