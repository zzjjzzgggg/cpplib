#ifndef __STRINGUTILS_H__
#define __STRINGUTILS_H__

#include <string>

#define FMT_HEADER_ONLY
#include <fmt/fmt/format.h>

namespace strutils {

void split_filename(const std::string &fullname,
                    std::string &base,
                    std::string &filename_wo_ext,
                    std::string &ext);

/**
 * ../syn/events2_U100_I10_T100.dat -->
 * ../syn/events2_U100_I10_T100_test.dat
 */
std::string insert_middle(const std::string &filename,
                          const std::string &sufix);

std::string get_base_path(const std::string &fullname);

std::string pretty_number(const int num);

static const double Kilobytes = 1 << 10, Megabytes = 1 << 20,
                    Gigabytes = 1 << 30;
std::string pretty_size(const int size);

std::string pretty_time(const double seconds);
}
#endif /* __STRINGUTILS_H__ */
