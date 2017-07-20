#ifndef __GZIPIO_H__
#define __GZIPIO_H__
// Compressed input and output streams. 7za.exe or 7z.exe must be in the path
// (http://www.7-zip.org)
//
// 7za.exe is a stand-alone program, which supports
//   -- extraction:  .gz, .7z, .rar, .zip, .cab, .arj. bzip2
//   -- compression: .7z
// 7z.exe uses DLLs in folders Codecs and Formats
//   -- extraction:  .gz, .7z, .rar, .zip, .cab, .arj. bzip2
//   -- compression: .gz, .7z, .rar, .zip, .cab, .arj. bzip2

#include <cstdio>
#include <cstring>

#include <string>
#include <unordered_set>

#include "iobase.h"
#include "../str/stringutils.h"

namespace ioutils {

class GZipOut : public IOOut {
private:
    static const size_t MAX_BUF_SIZE;
    size_t buf_sz_;
    char* buf_;
    FILE* zip_wr_;

private:
    void flush();

public:
    GZipOut(const std::string& filename);
    ~GZipOut();

    /**
     * Write `len` data from source `dat` to buffer. If the buffer is full, data
     * will be flushed to disk.
     */
    void write(const void* dat, const size_t len) override;

    void close() override;

    std::string getCmd(const std::string& filename);
};

class GZipIn : public IOIn {
private:
    static const size_t MAX_BUF_SIZE;

    size_t num_read_, buf_sz_;
    char* buf_;
    FILE* zip_rd_;

private:
    bool fill();

public:
    GZipIn(const std::string& filename);
    ~GZipIn();

    void close() override{};

    bool eof() override;

    /**
     * Read `len` data into target buffer `dat`. The method does not allocate
     * space for target buffer `dat`. It assumes the target buffer `dat` has at
     * least `len` space.
     */
    size_t read(const void* dat, const size_t len) override;

    /**
     * Read characters until a newline ('\n') is encountered. If a newline
     * character is not encountered in the first (len - 1) bytes, then the
     * excess characters are discarded. The returned string placed in 'data' is
     * null-terminated and includes the newline character if it was read in the
     * first (n - 1) bytes. The function return value is the number of bytes
     * placed in buffer (which includes the newline character if encountered,
     * but excludes the terminating null byte).
     */
    size_t readLine(const void* data, const size_t len) override;

    std::string getCmd(const std::string& zip_fnm) const;
};
}
#endif /* __GZIPIO_H__ */
