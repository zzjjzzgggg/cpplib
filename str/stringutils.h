#ifndef __STRINGUTILS_H__
#define __STRINGUTILS_H__

#include <string>
#include <sstream>

#define FMT_HEADER_ONLY
#include "../fmt/fmt/format.h"

namespace strutils {

/**
 * Split a full filename into three parts: base path, filename
 * without extension, and extension prefixed with '.'.
 * E.g., "~/work/test/cmp.sh" -> "~/work/test/", "cmp", ".sh"
 */
void splitFilename(const std::string &fullname, std::string &base,
                   std::string &fnm_wo_ext, std::string &ext);

/**
 * Insert a short string into the middle of a filename.
 * E.g., "./syn/event.dat", "test" --> "./syn/event_test.dat"
 */
std::string insertMiddle(const std::string &filename, const std::string &sufix);

/**
 * return the base path of fullname, with '/' at the tail
 */
std::string getBasePath(const std::string &fullname);

/**
 * Return a string representation of a number.
 * E.g., 1000 -> 1K; 10000 -> 10K
 */
std::string prettyNumber(const int num);

/**
 * Return a string representation of a size.
 * E.g., 100 -> 1B, 1024 -> 1K, 10240 -> 10K
 */
std::string prettySize(const int size);

/**
 * Split a string into parts by specified char.
 */
void split(const std::string &s, const char delim,
           std::vector<std::string> &elems);

std::vector<std::string> split(const std::string &s, const char delim);

std::string trim_left(const std::string &str);
std::string trim_right(const std::string &str);
std::string trim(const std::string &str);
}
#endif /* __STRINGUTILS_H__ */
