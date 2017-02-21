#ifndef __IOUTILS_H__
#define __IOUTILS_H__

#include <unordered_map>

#include "iobase.h"
#include "lz4io.h"
#include "gzipio.h"

namespace ioutils {

bool isGZip(const std::string& filename);

bool isLZ4(const std::string& filename);

std::unique_ptr<IOOut> getIOOut(const std::string& filename,
                                const bool append = false);

std::unique_ptr<IOIn> getIOIn(const std::string& filename);

class TSVParser {
private:
  size_t line_NO;
  char split_ch;
  char buf[1024];  // assume each line has a
                   // maximum length of 1024 bytes.
  std::vector<std::string> field_vec;
  std::unique_ptr<IOIn> in_ptr;

public:
  TSVParser(const std::string& filename)
      : line_NO(0), split_ch('\t') {
    in_ptr = getIOIn(filename);
  }

  TSVParser(const std::string& filename, const char sep)
      : line_NO(0), split_ch(sep) {
    in_ptr = getIOIn(filename);
  }

  bool next();

  int getNumFields() const { return (int)(field_vec.size()); }

  size_t getLineNO() const { return line_NO; }

  const std::string getLineString() const {
    return std::string(buf);
  }

  const char* getField(const int& id) const {
    return field_vec[id].c_str();
  }

  int getInt(const int& id) const;

  double getFlt(const int& id) const;
};

////////////////////////////////////////////////////////////
// vector saver
template <class TVal>
void saveVec(const std::vector<TVal>& vec,
             const std::string& filename,
             const std::string& line_fmt,
             const std::string& ano) {
  std::unique_ptr<IOOut> out_ptr = getIOOut(filename);
  if (!ano.empty()) out_ptr->save(ano);
  for (auto val : vec)
    out_ptr->save(fmt::format(line_fmt, val));
}

void saveIntVec(const std::vector<int>& vec,
                const std::string& filename,
                const std::string& ano = "");

void saveFltVec(const std::vector<double>& vec,
                const std::string& filename,
                const std::string& ano = "");

template <class T1, class T2>
void savePrVec(const std::vector<std::pair<T1, T2>>& vec,
               const std::string& filename,
               const std::string& line_fmt,
               const std::string& ano = "") {
  std::unique_ptr<IOOut> out_ptr = getIOOut(filename);
  if (!ano.empty()) out_ptr->save(ano);
  for (auto& pr : vec)
    out_ptr->save(fmt::format(line_fmt, pr.first, pr.second));
  out_ptr->close();
}

void saveIntPrVec(const std::vector<std::pair<int, int>>& vec,
                  const std::string& filename,
                  const std::string& ano = "");

void saveFltPrVec(
    const std::vector<std::pair<double, double>>& vec,
    const std::string& filename, const std::string& ano = "");

////////////////////////////////////////////////////////////
// vector loader
void loadIntVec(const std::string& filename,
                std::vector<int>& vec, const int col = 0);

void loadFltVec(const std::string& filename,
                std::vector<double>& vec, const int col = 0);

void loadIntPrVec(const std::string& filename,
                  std::vector<std::pair<int, int>>& vec,
                  const int c0 = 0, const int c1 = 1);

void loadFltPrVec(const std::string& filename,
                  std::vector<std::pair<double, double>>& vec,
                  const int c0 = 0, const int c1 = 1);

void loadIntFltPrVec(const std::string& filename,
                     std::vector<std::pair<int, double>>& vec,
                     const int c0 = 0, const int c1 = 1);

////////////////////////////////////////////////////////////
// map saver
template <class TKey, class TVal>
void saveMap(const std::unordered_map<TKey, TVal>& mp,
             const std::string& filename,
             const std::string& line_fmt,
             const std::string& ano) {
  std::unique_ptr<IOOut> out_ptr = getIOOut(filename);
  if (!ano.empty()) out_ptr->save(ano);
  for (auto& pr : mp)
    out_ptr->save(fmt::format(line_fmt, pr.first, pr.second));
}

void saveIntMap(const std::unordered_map<int, int>& mp,
                const std::string& filename,
                const std::string& ano = "");

void saveIntFltMap(const std::unordered_map<int, double>& mp,
                   const std::string& filename,
                   const std::string& ano = "");

////////////////////////////////////////////////////////////
// map loader

void loadIntMap(const std::string& filename,
                std::unordered_map<int, int>& mp,
                const int kc = 0, const int vc = 1);

void loadIntFltMap(const std::string& filename,
                   std::unordered_map<int, double>& mp,
                   const int kc = 0, const int vc = 1);

} /* namespace ioutils */

#endif /* __IOUTILS_H__ */
