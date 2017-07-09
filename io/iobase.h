#ifndef __IOBASE_H__
#define __IOBASE_H__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

namespace ioutils {

class IOOut {
public:
    virtual ~IOOut() = default;
    virtual void close() = 0;
    virtual void write(const void* data, const size_t length) = 0;

    virtual void save(const int val) { write(&val, sizeof(int)); }
    virtual void save(const long val) { write(&val, sizeof(long)); }
    virtual void save(const double val) { write(&val, sizeof(double)); };
    virtual void save(const char* str) { write(str, strlen(str)); }
    virtual void save(const std::string& str) {
        write(str.c_str(), str.size());
    }
};

class IOIn {
public:
    virtual ~IOIn() = default;
    virtual void close() = 0;
    virtual bool eof() = 0;

    /**
     * Read characters of length at most len bytes.
     * The returned string placed in 'data' is *not*
     * null-terminated, and is of length at most 'len'.
     * The function return value is the actual number of bytes
     * placed in buffer.
     */
    virtual size_t read(const void* data, const size_t len) = 0;

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
    virtual size_t readLine(const void* data, const size_t len) = 0;

    virtual void load(int& val) { read(&val, sizeof(int)); }
    virtual void load(long& val) { read(&val, sizeof(long)); }
    virtual void load(double& val) { read(&val, sizeof(double)); }
};

class NormOut : public IOOut {
private:
    FILE* fw_ = nullptr;

public:
    NormOut(const std::string& filename, const bool append) {
        const char* mode = append ? "ab" : "wb";
        fw_ = std::fopen(filename.c_str(), mode);
        if (fw_ == nullptr) {
            std::fprintf(stderr, "Open file '%s' failed!\n", filename.c_str());
            exit(1);
        }
    }
    virtual ~NormOut() { close(); }

    void close() override {
        if (fw_ != nullptr) {
            fclose(fw_);
            fw_ = nullptr;
        }
    }
    void write(const void* data, const size_t length) override {
        std::fwrite(data, 1, length, fw_);
    }
};

class NormIn : public IOIn {
private:
    FILE* fr_ = nullptr;

public:
    NormIn(const std::string& filename) {
        fr_ = std::fopen(filename.c_str(), "r");
        if (fr_ == NULL) {
            std::fprintf(stderr, "Open file '%s' failed!\n", filename.c_str());
            exit(1);
        }
    }

    virtual ~NormIn() { close(); }

    void close() override {
        if (fr_ != nullptr) {
            std::fclose(fr_);
            fr_ = nullptr;
        }
    }

    bool eof() override { return feof(fr_); }

    size_t read(const void* data, const size_t len) override {
        return std::fread(const_cast<void*>(data), 1, len, fr_);
    }

    size_t readLine(const void* data, const size_t len) override {
        char* dst = (char*)data;
        if (fgets(dst, len, fr_) != NULL)
            return strlen(dst);
        else
            return 0;
    }
};
}

#endif /* __IOBASE_H__ */
