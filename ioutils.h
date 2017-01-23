#ifndef __IOUTILS_H__
#define __IOUTILS_H__

#include <gzipio.h>

template <TKey key, TVal val>
saveMap(std::unordered_map<TKey, TVal> map_to_save,
        std::string zip_filename) {
  GZipOut gzo(zip_filename);
  for (auto& pr : map_to_save) {
    gzo.save(fmt::format("{}\t{}\n", source, id, pr.second)
                 .c_str());
  }
}

#endif /* __IOUTILS_H__ */
