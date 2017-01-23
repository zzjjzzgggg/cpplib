#include "gzipio.h"

const std::unordered_set<std::string> GZipOut::gzip_ext_set{
    {".gz", ".zip", ".7z", ".bzip2", ".bz2"}};
const size_t GZipOut::MxBfL = 4 * 1024;

void GZipOut::FlushBf() {
  size_t BytesOut = fwrite(Bf, 1, BfL, ZipStdinWr);
  assert(BytesOut == BfL);
  BfL = 0;
}

void GZipOut::CreateZipProcess(const std::string& cmd,
                               const std::string& zip_fnm) {
  std::string cmd_line =
      fmt::format("{} {}", cmd.c_str(), zip_fnm.c_str());
  cmd_line += " >/dev/null";
  ZipStdinWr = popen(cmd_line.c_str(), "w");
  assert_msg(ZipStdinWr != NULL, "Can not execute '%s'",
             cmd_line.c_str());
}

GZipOut::GZipOut(const std::string& filename)
    : ZipStdinRd(NULL), ZipStdinWr(NULL), Bf(NULL), BfL(0) {
  CreateZipProcess(getCmd(filename), filename);
  Bf = new char[MxBfL];
  BfL = 0;
}

GZipOut::~GZipOut() {
  close();
  if (Bf != NULL) delete[] Bf;
}

void GZipOut::close() {
  if (BfL != 0) FlushBf();
  if (ZipStdinWr != NULL) {
    assert_msg(pclose(ZipStdinWr) != -1,
               "Closing of the process failed");
    ZipStdinWr = NULL;
  }
}

int GZipOut::putChar(const char& Ch) {
  if (BfL == MxBfL) FlushBf();
  return Bf[BfL++] = Ch;
}

void GZipOut::write(const void* data, const size_t length) {
  if (BfL + length > MxBfL) {
    for (size_t LBfC = 0; LBfC < length; LBfC++)
      putChar(((char*)data)[LBfC]);
  } else {
    for (size_t LBfC = 0; LBfC < length; LBfC++)
      Bf[BfL++] = ((char*)data)[LBfC];
  }
}

void GZipOut::Flush() { FlushBf(); }

bool GZipOut::isZip(const std::string& filename) {
  std::string base, name, ext;
  stringutils::splitFilename(filename, base, name, ext);
  return gzip_ext_set.find(ext) != gzip_ext_set.end();
}

std::string GZipOut::getCmd(const std::string& zip_fnm) {
  std::string base, name, ext;
  stringutils::splitFilename(zip_fnm, base, name, ext);
  assert_msg(gzip_ext_set.find(ext) != gzip_ext_set.end(),
             "Unknown file extension '%s'.", ext.c_str());
  return "7za a -y -bd -si" + name;
}

/////////////////////////////////////////////////////////
const std::unordered_set<std::string> GZipIn::gzip_ext_set{
    {".gz", ".zip", ".7z", ".bzip2", ".bz2"}};
const int GZipIn::MxBfL = 32 * 1024;

void GZipIn::CreateZipProcess(const std::string& cmd,
                              const std::string& zip_fnm) {
  std::string cmd_line =
      fmt::format("{} {}", cmd.c_str(), zip_fnm.c_str());
  cmd_line += " 2>/dev/null";
  ZipStdoutRd = popen(cmd_line.c_str(), "r");
  assert_msg(ZipStdoutRd != NULL, "Can not execute '%s'",
             cmd_line.c_str());
}

void GZipIn::FillBf() {
  size_t BytesRead = fread(Bf, 1, MxBfL, ZipStdoutRd);
  BfL = (int)BytesRead;
  CurFPos += BytesRead;
  BfC = 0;
}

GZipIn::GZipIn(const std::string& filename)
    : ZipStdoutRd(NULL), ZipStdoutWr(NULL), CurFPos(0),
      Bf(NULL), BfC(0), BfL(0) {
  CreateZipProcess(getCmd(filename), filename);
  Bf = new char[MxBfL];
  BfC = BfL = -1;
  FillBf();
}

GZipIn::~GZipIn() {
  if (ZipStdoutRd != NULL)
    assert_msg(pclose(ZipStdoutRd) != -1,
               "Closing of the process failed");
  if (Bf != NULL) delete[] Bf;
}

size_t GZipIn::read(const void* LBf, const size_t LBfL) {
  size_t LBfS = 0;
  if (BfC + LBfL > BfL) {
    for (size_t LBfC = 0; LBfC < LBfL; LBfC++) {
      if (BfC == BfL) FillBf();
      LBfS += ((char*)LBf)[LBfC] = Bf[BfC++];
    }
  } else {
    for (size_t LBfC = 0; LBfC < LBfL; LBfC++)
      LBfS += (((char*)LBf)[LBfC] = Bf[BfC++]);
  }
  return LBfS;
}

bool GZipIn::isZip(const std::string& filename) {
  std::string base, name, ext;
  stringutils::splitFilename(filename, base, name, ext);
  return gzip_ext_set.find(ext) != gzip_ext_set.end();
}

std::string GZipIn::getCmd(const std::string& zip_fnm) {
  assert_msg(isZip(zip_fnm),
             "Unsupported file extension '%s'",
             zip_fnm.c_str());
  return "7za e -y -bd -so";
}
