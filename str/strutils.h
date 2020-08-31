#ifndef __STRUTILS_H__
#define __STRUTILS_H__

#include <string>
#include <sstream>

#define FMT_HEADER_ONLY
#include "../fmt/fmt/format.h"

namespace strutils {

/**
 * Split a full filename into three parts: base path, filename
 * without extension, and extension prefixed with '.'.
 *
 * E.g., "~/work/test/cmp.sh" -> "~/work/test/", "cmp", ".sh"
 */
void splitFilename(const std::string &fullname, std::string &base,
                   std::string &fnm_wo_ext, std::string &ext);

/**
 * Insert a short string into the middle of "filename" with the given extension
 * "ext". If "ext" is empty, the file extension does not change.
 *
 * E.g., "./syn/event.dat", "test"       --> "./syn/event_test.dat"
 *       "./syn/event.dat", "test", "gz" --> "./syn/event_test.gz"
 */
std::string insertMiddle(const std::string &filename, const std::string &sufix,
                         const std::string &ext = "");

/**
 * Return the base path of fullname, with '/' at the tail
 */
std::string getBasePath(const std::string &fullname);

/**
 * Substitute the filename in fullname with given filename.
 */
std::string subFilename(const std::string &fullname, const std::string &filename);

/**
 * Substitute the file extension.
 *
 * E.g., "event.dat", ".txt"   -->   "event.txt"
 *       "event",     ".txt"   -->   "event.txt"
 */
std::string subFileExt(const std::string &filename, const std::string &ext);

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
}  // namespace strutils
#endif /* __STRUTILS_H__ */
