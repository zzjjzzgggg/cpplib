#ifndef __STRINGUTILS_H__
#define __STRINGUTILS_H__

#include <string>
#include <sstream>

#define FMT_HEADER_ONLY
#include <fmt/fmt/format.h>

namespace stringutils {


// ~/workspace/test/cmp.sh
// base: ~/workspace/test/
// filename_wo_ext: cmp
// ext: .sh
void splitFilename(const std::string &fullname,
                   std::string &base,
                   std::string &fnm_wo_ext,
                   std::string &ext);

/**
 * ../syn/events2_U100_I10_T100.dat -->
 * ../syn/events2_U100_I10_T100_test.dat
 */
std::string insertMiddle(const std::string &filename,
                         const std::string &sufix);

/**
 * return the base path of fullname, with '/' at the tail
 */
std::string getBasePath(const std::string &fullname);

inline char pathSeparator() {
#ifdef _WIN32
  return '\\';
#else
  return '/';
#endif
}

std::string joinPath(const std::string& parent,
                     const std::string& child);

std::string prettyNumber(const int num);

static const double Kilobytes = 1 << 10, Megabytes = 1 << 20,
                    Gigabytes = 1 << 30;
std::string prettySize(const int size);

std::string prettyTime(const double seconds);

void split(const std::string &s, char delim,
           std::vector<std::string> &elems);

std::vector<std::string> split(const std::string &s,
                               const char delim);
}
#endif /* __STRINGUTILS_H__ */
