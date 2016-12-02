#ifndef __STRINGUTILS_H__
#define __STRINGUTILS_H__

#include <string>
#include <sstream>

#define FMT_HEADER_ONLY
#include <fmt/fmt/format.h>

namespace strutils {

void splitFilename(const std::string &fullname,
                    std::string &base,
                    std::string &filename_wo_ext,
                    std::string &ext);

/**
 * ../syn/events2_U100_I10_T100.dat -->
 * ../syn/events2_U100_I10_T100_test.dat
 */
std::string insertMiddle(const std::string &filename,
                          const std::string &sufix);

std::string getBasePath(const std::string &fullname);

std::string prettyNumber(const int num);

static const double Kilobytes = 1 << 10, Megabytes = 1 << 20,
                    Gigabytes = 1 << 30;
std::string prettySize(const int size);

std::string prettyTime(const double seconds);

void split(const std::string &s, char delim,
           std::vector<std::string> &elems);

std::vector<std::string> split(const std::string &s, char delim);
}
#endif /* __STRINGUTILS_H__ */
