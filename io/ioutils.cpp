#include "ioutils.h"

namespace ioutils {

bool isGZip(const std::string& filename) {
  std::unordered_set<std::string> gzip_exts;
  gzip_exts.insert(".gz");
  gzip_exts.insert(".zip");
  std::string base, name, ext;
  stringutils::splitFilename(filename, base, name, ext);
  return gzip_exts.find(ext) != gzip_exts.end();
}

bool isLZ4(const std::string& filename) {
  std::unordered_set<std::string> lz4_exts;
  lz4_exts.insert(".lz");
  lz4_exts.insert(".lz4");
  std::string base, name, ext;
  stringutils::splitFilename(filename, base, name, ext);
  return lz4_exts.find(ext) != lz4_exts.end();
  return true;
}

std::unique_ptr<IOOut> getIOOut(const std::string& filename) {
  if (isGZip(filename))
    return std::make_unique<GZipOut>(filename);
  else if (isLZ4(filename))
    return std::make_unique<LZ4Out>(filename.c_str());
  return std::make_unique<NormOut>(filename);
}

std::unique_ptr<IOIn> getIOIn(const std::string& filename) {
  if (isGZip(filename))
    return std::make_unique<GZipIn>(filename);
  else if (isLZ4(filename))
    return std::make_unique<LZ4In>(filename.c_str());
  return std::make_unique<NormIn>(filename);
}

bool TSVParser::next() {
  field_vec.clear();
  line_NO++;
  if (in_ptr->readLine(buf, 1024) == 0) return false;
  if (buf[0] == '#') return next();
  stringutils::split(buf, split_ch, field_vec);
  return true;
}

int TSVParser::getInt(const int& id) const {
  int val = 0;
  try {
    val = std::stoi(field_vec[id]);
  } catch (const std::invalid_argument& err) {
    fprintf(stderr, "Field %d in line %lu is not integer\n",
            id, line_NO);
  }
  return val;
}

double TSVParser::getFlt(const int& id) const {
  double val = 0.0;
  try {
    val = std::stod(field_vec[id]);
  } catch (const std::invalid_argument& err) {
    fprintf(stderr, "Field %d in line %lu is not double\n", id,
            line_NO);
  }
  return val;
}

void saveIntVec(const std::vector<int>& vec,
                const std::string& filename,
                const std::string& ano) {
  saveVec(vec, filename, "{}\n", ano);
}

void saveFltVec(const std::vector<double>& vec,
                const std::string& filename,
                const std::string& ano) {
  saveVec(vec, filename, "{:.6e}\n", ano);
}

void loadIntVec(const std::string& filename,
                std::vector<int>& vec, const int col) {
  TSVParser ss(filename);
  while (ss.next()) vec.push_back(ss.getInt(col));
}

void loadFltVec(const std::string& filename,
                std::vector<double>& vec, const int col) {
  TSVParser ss(filename);
  while (ss.next()) vec.push_back(ss.getFlt(col));
}

void saveIntPrVec(const std::vector<std::pair<int, int>>& vec,
                  const std::string& filename,
                  const std::string& ano) {
  savePrVec(vec, filename, "{}\t{}\n", ano);
}

void saveFltPrVec(
    const std::vector<std::pair<double, double>>& vec,
    const std::string& filename, const std::string& ano) {
  savePrVec(vec, filename, "{:.6e}\t{:.6e}\n", ano);
}

void loadIntPrVec(const std::string& filename,
                  std::vector<std::pair<int, int>>& vec,
                  const int c0, const int c1) {
  TSVParser ss(filename);
  while (ss.next())
    vec.emplace_back(ss.getInt(c0), ss.getInt(c1));
}

void loadFltPrVec(const std::string& filename,
                  std::vector<std::pair<double, double>>& vec,
                  const int c0, const int c1) {
  TSVParser ss(filename);
  while (ss.next())
    vec.emplace_back(ss.getFlt(c0), ss.getFlt(c1));
}
}