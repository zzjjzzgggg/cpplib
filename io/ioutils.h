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
    TSVParser(const std::string& filename) : line_NO(0), split_ch('\t') {
        in_ptr = getIOIn(filename);
    }

    TSVParser(const std::string& filename, const char sep)
        : line_NO(0), split_ch(sep) {
        in_ptr = getIOIn(filename);
    }

    bool next();

    int getNumFields() const { return (int)(field_vec.size()); }

    size_t getLineNO() const { return line_NO; }

    const std::string getLine() const { return std::string(buf); }

    template <typename T>
    auto get(const int& id) const -> T;
};

// vector saver
template <class TVal>
void saveVec(const std::vector<TVal>& vec, const std::string& filename,
             const std::string& ano = "") {
    std::unique_ptr<IOOut> out_ptr = getIOOut(filename);
    if (!ano.empty()) out_ptr->save(ano);
    for (auto val : vec) out_ptr->save(fmt::format("{}\n", val));
}

template <class T1, class T2>
void savePrVec(const std::vector<std::pair<T1, T2>>& vec,
               const std::string& filename, const std::string& ano = "") {
    std::unique_ptr<IOOut> out_ptr = getIOOut(filename);
    if (!ano.empty()) out_ptr->save(ano);
    for (auto& pr : vec)
        out_ptr->save(fmt::format("{}\t{}\n", pr.first, pr.second));
    out_ptr->close();
}

// vector loader
template <class T>
void loadVec(const std::string& filename, std::vector<T>& vec,
             const int col = 0) {
    TSVParser ss(filename);
    while (ss.next()) vec.push_back(ss.get<T>(col));
}

template <class T1, class T2>
void loadPrVec(const std::string& filename, std::vector<std::pair<T1, T2>>& vec,
               const int c0 = 0, const int c1 = 1) {
    TSVParser ss(filename);
    while (ss.next()) vec.emplace_back(ss.get<T1>(c0), ss.get<T2>(c1));
}

// map saver
template <class TKey, class TVal>
void saveMap(const std::unordered_map<TKey, TVal>& mp,
             const std::string& filename, const std::string& ano = "") {
    std::unique_ptr<IOOut> out_ptr = getIOOut(filename);
    if (!ano.empty()) out_ptr->save(ano);
    for (auto& pr : mp)
        out_ptr->save(fmt::format("{}\t{}\n", pr.first, pr.second));
}

// map loader
template <class TKey, class TVal>
void loadMap(const std::string& filename, std::unordered_map<TKey, TVal>& mp,
             const int kc = 0, const int vc = 1) {
    TSVParser ss(filename);
    while (ss.next()) mp[ss.get<TKey>(kc)] = ss.get<TVal>(vc);
}

} /* namespace ioutils */

#endif /* __IOUTILS_H__ */
