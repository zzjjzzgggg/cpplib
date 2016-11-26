#ifndef __STRINGUTILS_H__
#define __STRINGUTILS_H__

#include <string>

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
}
#endif /* __STRINGUTILS_H__ */
