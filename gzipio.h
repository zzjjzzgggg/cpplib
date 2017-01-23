#ifndef __GZIPIO_H__
#define __GZIPIO_H__
// Compressed input and output streams.
// 7za.exe or 7z.exe must be in the path
// (http://www.7-zip.org)
// 7za.exe is a stand-alone program, which supports
//   -- extraction:  .gz, .7z, .rar, .zip, .cab, .arj. bzip2
//   -- compression: .7z
// 7z.exe uses DLLs in folders Codecs and Formats
//   -- extraction:  .gz, .7z, .rar, .zip, .cab, .arj. bzip2
//   -- compression: .gz, .7z, .rar, .zip, .cab, .arj. bzip2

#include <cstdio>
#include <cassert>
#include <cstring>

#include <unordered_set>
#include <stringutils.h>

#include <ppk_assert.h>
#define assert_msg PPK_ASSERT

class GZipOut {
private:
  static const size_t MxBfL;
  static const std::unordered_set<std::string> gzip_ext_set;
  FILE *ZipStdinRd, *ZipStdinWr;
  char* Bf;
  size_t BfL;

private:
  void FlushBf();
  void CreateZipProcess(const std::string& cmd,
                        const std::string& filename);

public:
  GZipOut(const std::string& filename);
  ~GZipOut();

  int putChar(const char& ch);
  void write(const void* data, const size_t length);
  void save(const char* str) { write(str, strlen(str)); }
  void save(const int val) { write(&val, sizeof(int)); }
  void save(const long val) { write(&val, sizeof(long)); }
  void save(const double val) { write(&val, sizeof(double)); }

  void Flush();
  void close();

  static bool isZip(const std::string& filename);
  std::string getCmd(const std::string& filename);
};

class GZipIn {
private:
  static const int MxBfL;
  static const std::unordered_set<std::string> gzip_ext_set;
  FILE *ZipStdoutRd, *ZipStdoutWr;
  size_t CurFPos;
  char* Bf;
  int BfC, BfL;

private:
  void FillBf();
  void CreateZipProcess(const std::string& cmd,
                        const std::string& zip_fnm);

public:
  GZipIn(const std::string& filename);
  ~GZipIn();

  bool eof() { return BfL < MxBfL && BfC == BfL; }
  char getChar() {
    if (BfC == BfL) FillBf();
    return Bf[BfC++];
  }

  size_t read(const void* LBf, const size_t LBfL);
  void load(int& val) { read(&val, sizeof(int)); }
  void load(long& val) { read(&val, sizeof(long)); }
  void load(double& val) { read(&val, sizeof(double)); }

  std::string getCmd(const std::string& zip_fnm);
  static bool isZip(const std::string& filename);
};

#endif /* __GZIPIO_H__ */
