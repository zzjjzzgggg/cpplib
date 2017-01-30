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

#include <string>
#include <unordered_set>

#include "iobase.h"
#include "../str/stringutils.h"
#include "../Assert/src/ppk_assert.h"
#define assert_msg PPK_ASSERT

namespace ioutils {

class GZipOut : public IOOut {
private:
  static const size_t MxBfL;
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

  void write(const void* data, const size_t length) override;
  void close() override;

  void Flush();

  std::string getCmd(const std::string& filename);
};

class GZipIn : public IOIn {
private:
  static const int MxBfL;
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

  void close() override {};

  bool eof() override { return BfL < MxBfL && BfC == BfL; }
  char getChar() {
    if (BfC == BfL) FillBf();
    return Bf[BfC++];
  }

  size_t read(const void* LBf, const size_t LBfL) override;

  /**
   * Read characters until a newline ('\n') is encountered.
   * If a newline character is not encountered in the first
   * (len - 1) bytes, then the excess characters are
   * discarded.
   * The returned string placed in 'data' is null-terminated
   * and includes the newline character if it was read in the
   * first (n - 1) bytes.
   * The function return value is the number of bytes placed
   * in buffer (which includes the newline character if
   * encountered, but excludes the terminating null byte).
   **/
  size_t readLine(const void* data, const size_t len) override;

  std::string getCmd(const std::string& zip_fnm);
};
}
#endif /* __GZIPIO_H__ */
