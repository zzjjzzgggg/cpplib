#ifndef __IOUTILS_H__
#define __IOUTILS_H__

#include <unordered_map>

#include "iobase.h"
#include "lz4io.h"
#include "gzipio.h"
#include "../os/osutils.h"

namespace ioutils {

bool isGZip(const std::string& filename);

bool isLZ4(const std::string& filename);

std::unique_ptr<IOOut> getIOOut(const std::string& filename,
                                const bool append = false);

std::unique_ptr<IOIn> getIOIn(const std::string& filename);

class TSVParser {
private:
    size_t line_NO_;
    char split_ch_;
    char buf_[1024];  // assume each line has a maximum length of 1024 bytes.
    std::vector<std::string> field_vec_;
    std::unique_ptr<IOIn> in_ptr_;

public:
    TSVParser(const std::string& filename) : line_NO_(0), split_ch_('\t') {
        in_ptr_ = getIOIn(filename);
    }

    TSVParser(const std::string& filename, const char sep)
        : line_NO_(0), split_ch_(sep) {
        in_ptr_ = getIOIn(filename);
    }

    bool next();

    int getNumFields() const { return (int)(field_vec_.size()); }

    size_t getLineNO() const { return line_NO_; }

    const std::string getLine() const { return std::string(buf_); }

    template <typename T>
    auto get(const int& id) const -> T;
};

// vector
template <typename TVal>
void saveVec(const std::vector<TVal>& vec, const std::string& filename,
             const bool echo = true, const std::string& format = "{}\n",
             const std::string& anno = "") {
    std::unique_ptr<IOOut> out_ptr = getIOOut(filename);
    if (!anno.empty()) out_ptr->save(anno);
    for (auto val : vec) out_ptr->save(fmt::format(format, val));
    if (echo) printf("saved to %s\n", filename.c_str());
}

template <typename T>
void loadVec(const std::string& filename, std::vector<T>& vec,
             const int col = 0) {
    TSVParser ss(filename);
    while (ss.next()) vec.push_back(ss.get<T>(col));
}

template <typename T>
std::vector<T> loadVec(const std::string& filename, const int col = 0) {
    std::vector<T> vec;
    loadVec(filename, vec, col);
    return vec;
}

// vector pair
template <typename T1, typename T2>
void savePrVec(const std::vector<std::pair<T1, T2>>& vec,
               const std::string& filename, const bool echo = true,
               const std::string& format = "{}\t{}\n",
               const std::string& anno = "") {
    std::unique_ptr<IOOut> out_ptr = getIOOut(filename);
    if (!anno.empty()) out_ptr->save(anno);
    for (auto& pr : vec)
        out_ptr->save(fmt::format(format, pr.first, pr.second));
    out_ptr->close();
    if (echo) printf("saved to %s\n", filename.c_str());
}

template <typename T1, typename T2>
void loadPrVec(const std::string& filename, std::vector<std::pair<T1, T2>>& vec,
               const int c0 = 0, const int c1 = 1) {
    TSVParser ss(filename);
    while (ss.next()) vec.emplace_back(ss.get<T1>(c0), ss.get<T2>(c1));
}

template <typename T1, typename T2>
std::vector<std::pair<T1, T2>> loadPrVec(const std::string& filename,
                                         const int c0 = 0, const int c1 = 1) {
    std::vector<std::pair<T1, T2>> vec;
    loadPrVec(filename, vec, c0, c1);
    return vec;
}

// Triplet vector
template <typename T1, typename T2, typename T3>
void saveTripletVec(const std::vector<std::tuple<T1, T2, T3>>& vec,
                    const std::string& filename, const bool echo = true,
                    const std::string& format = "{}\t{}\t{}\n",
                    const std::string& anno = "") {
    std::unique_ptr<IOOut> out_ptr = getIOOut(filename);
    if (!anno.empty()) out_ptr->save(anno);
    for (auto&& e : vec)
        out_ptr->save(fmt::format(format, std::get<0>(e), std::get<1>(e),
                                  std::get<2>(e)));
    out_ptr->close();
    if (echo) printf("saved to %s\n", filename.c_str());
}

template <typename T1, typename T2, typename T3>
void loadTripletVec(const std::string& filename,
                    std::vector<std::tuple<T1, T2, T3>>& vec, const int c0 = 0,
                    const int c1 = 1, const int c2 = 2) {
    TSVParser ss(filename);
    while (ss.next())
        vec.emplace_back(ss.get<T1>(c0), ss.get<T2>(c1), ss.get<T3>(c2));
}

template <typename T1, typename T2, typename T3>
std::vector<std::tuple<T1, T2, T3>> loadTripletVec(const std::string& filename,
                                                   const int c0 = 0,
                                                   const int c1 = 1,
                                                   const int c2 = 2) {
    std::vector<std::tuple<T1, T2, T3>> vec;
    loadTripletVec(filename, vec, c0, c1, c2);
    return vec;
}

// Quadruplet vector
template <typename T1, typename T2, typename T3, typename T4>
void saveQuadrupletVec(const std::vector<std::tuple<T1, T2, T3, T4>>& vec,
                       const std::string& filename, const bool echo = true,
                       const std::string& format = "{}\t{}\t{}\t{}\n",
                       const std::string& anno = "") {
    std::unique_ptr<IOOut> out_ptr = getIOOut(filename);
    if (!anno.empty()) out_ptr->save(anno);
    for (auto&& e : vec)
        out_ptr->save(fmt::format(format, std::get<0>(e), std::get<1>(e),
                                  std::get<2>(e), std::get<3>(e)));
    out_ptr->close();
    if (echo) printf("saved to %s\n", filename.c_str());
}

template <typename T1, typename T2, typename T3, typename T4>
void loadQuadrupletVec(const std::string& filename,
                       std::vector<std::tuple<T1, T2, T3, T4>>& vec,
                       const int c0 = 0, const int c1 = 1, const int c2 = 2,
                       const int c3 = 3) {
    TSVParser ss(filename);
    while (ss.next())
        vec.emplace_back(ss.get<T1>(c0), ss.get<T2>(c1), ss.get<T3>(c2),
                         ss.get<T4>(c3));
}

template <typename T1, typename T2, typename T3, typename T4>
std::vector<std::tuple<T1, T2, T3, T4>> loadQuadrupletVec(
    const std::string& filename, const int c0 = 0, const int c1 = 1,
    const int c2 = 2, const int c3 = 3) {
    std::vector<std::tuple<T1, T2, T3, T4>> vec;
    loadTripletVec(filename, vec, c0, c1, c2, c3);
    return vec;
}

// map
template <typename TKey, typename TVal>
void saveMap(const std::unordered_map<TKey, TVal>& map,
             const std::string& filename, const bool echo = true,
             const std::string& format = "{}\t{}\n",
             const std::string& anno = "") {
    std::unique_ptr<IOOut> out_ptr = getIOOut(filename);
    if (!anno.empty()) out_ptr->save(anno);
    for (auto& pr : map)
        out_ptr->save(fmt::format(format, pr.first, pr.second));
    if (echo) printf("saved to %s\n", filename.c_str());
}

template <typename TKey, typename TVal>
void loadMap(const std::string& filename, std::unordered_map<TKey, TVal>& map,
             const int kc = 0, const int vc = 1) {
    TSVParser ss(filename);
    while (ss.next()) map[ss.get<TKey>(kc)] = ss.get<TVal>(vc);
}

template <typename TKey, typename TVal>
std::unordered_map<TKey, TVal> loadMap(const std::string& filename,
                                       const int kc = 0, const int vc = 1) {
    std::unordered_map<TKey, TVal> map;
    loadMap(filename, map, kc, vc);
    return map;
}

// set
template <typename T>
void saveSet(const std::unordered_set<T>& set, const std::string& filename,
             const bool echo = true, const std::string& format = "{}\n",
             const std::string& anno = "") {
    std::unique_ptr<IOOut> out_ptr = getIOOut(filename);
    if (!anno.empty()) out_ptr->save(anno);
    for (auto val : set) out_ptr->save(fmt::format(format, val));
    if (echo) printf("saved to %s\n", filename.c_str());
}

template <typename TKey>
void loadSet(const std::string& filename, std::unordered_set<TKey>& set,
             const int c = 0) {
    TSVParser ss(filename);
    while (ss.next()) set.insert(ss.get<TKey>(c));
}

template <typename TKey>
std::unordered_set<TKey> loadSet(const std::string& filename, const int c = 0) {
    std::unordered_set<TKey> set;
    loadSet(filename, set, c);
    return set;
}

} /* namespace ioutils */

#endif /* __IOUTILS_H__ */
