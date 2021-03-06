#include "ioutils.h"

namespace ioutils {

bool isGZip(const std::string& filename) {
    std::unordered_set<std::string> gzip_exts;
    gzip_exts.insert(".gz");
    gzip_exts.insert(".zip");
    std::string base, name, ext;
    strutils::splitFilename(filename, base, name, ext);
    return gzip_exts.find(ext) != gzip_exts.end();
}

bool isLZ4(const std::string& filename) {
    std::unordered_set<std::string> lz4_exts;
    lz4_exts.insert(".lz");
    lz4_exts.insert(".lz4");
    std::string base, name, ext;
    strutils::splitFilename(filename, base, name, ext);
    return lz4_exts.find(ext) != lz4_exts.end();
    return true;
}

std::unique_ptr<IOOut> getIOOut(const std::string& filename,
                                const bool append) {
    osutils::rmfile(filename);
    if (isGZip(filename))
        return std::make_unique<GZipOut>(filename);
    else if (isLZ4(filename))
        return std::make_unique<LZ4Out>(filename.c_str(), append);
    return std::make_unique<NormOut>(filename, append);
}

/**
 * Get file reader handler.
 * If file does not exists, return a nullptr.
 */
std::unique_ptr<IOIn> getIOIn(const std::string& filename) {
    if (!osutils::exists(filename)) return nullptr;
    if (isGZip(filename))
        return std::make_unique<GZipIn>(filename);
    else if (isLZ4(filename))
        return std::make_unique<LZ4In>(filename.c_str());
    return std::make_unique<NormIn>(filename);
}

bool TSVParser::next() {
    field_vec_.clear();
    line_NO_++;
    if (in_ptr_->readLine(buf_, 1024) == 0) return false;
    if (buf_[0] == '#') return next();
    strutils::split(buf_, split_ch_, field_vec_);
    return true;
}

template <>
auto TSVParser::get<int>(const int& id) const -> int {
    return std::stoi(field_vec_[id]);
}

template <>
auto TSVParser::get<float>(const int& id) const -> float {
    return std::stof(field_vec_[id]);
}

template <>
auto TSVParser::get<double>(const int& id) const -> double {
    return std::stod(field_vec_[id]);
}

template <>
auto TSVParser::get<std::string>(const int& id) const -> std::string {
    return strutils::trim(field_vec_[id]);
}

}  // end namespace ioutils
