#ifndef __LZ4IO_H__
#define __LZ4IO_H__

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "iobase.h"
#include "../lz4/lib/lz4.h"

namespace ioutils {

enum {
    BLOCK_BYTES = 65536,  // 64KB
};

static const size_t DATA_CAPACITY = BLOCK_BYTES;
static const size_t CHUNK_CAPACITY = LZ4_COMPRESSBOUND(BLOCK_BYTES);

class LZ4Out : public IOOut {
private:
    char *data_buf_ = nullptr, *chunk_buf_ = nullptr;
    size_t len_dat_;
    FILE* output_ = nullptr;

private:
    /**
     * Compress the data in internal data buffer, and compressed data is
     * buffered in the internal chunk buffer. A compressed block has the
     * following structure:
     *
     * +-----------------+------+-----------------+------+----
     * | compressed-size | data | compressed-size | data | ...
     * +-----------------+------+-----------------+------+----
     *
     */
    void writeChunk();

public:
    LZ4Out();
    LZ4Out(const char* file_name, const bool append = false) : LZ4Out() {
        open(file_name, append);
    }
    virtual ~LZ4Out();

    // disable copy constructor
    LZ4Out(const LZ4Out&) = delete;

    // disable copy assignment
    LZ4Out& operator=(const LZ4Out&) = delete;

    // move constructor
    LZ4Out(LZ4Out&& other)
        : data_buf_(std::move(other.data_buf_)),
          chunk_buf_(std::move(other.chunk_buf_)),
          len_dat_(std::move(other.len_dat_)),
          output_(std::move(other.output_)) {
        other.data_buf_ = other.chunk_buf_ = nullptr;
        other.output_ = nullptr;
        other.len_dat_ = 0;
    }

    // move assignment
    LZ4Out& operator=(LZ4Out&& other) {
        data_buf_ = std::move(other.data_buf_);
        chunk_buf_ = std::move(other.chunk_buf_);
        len_dat_ = std::move(other.len_dat_);
        output_ = std::move(other.output_);
        other.len_dat_ = 0;
        other.data_buf_ = other.chunk_buf_ = nullptr;
        other.output_ = nullptr;
        return *this;
    }

    void open(const char* file_name, const bool append = false);

    /**
     * Add 'data' of 'length' to internal data buffer. The data length may be
     * larger than the internal buffer size. If the internal data buffer is
     * full, a compression operation is triggered automatically, and the
     * internal data buffer is then emptified.
     */
    void write(const void* data, const size_t length) override;

    void close() override;
    bool isClosed() const { return output_ == nullptr; }

    void compress(const char* input_file_name);
};

class LZ4In : public IOIn {
private:
    char *chunk_buf_ = nullptr, *data_buf_ = nullptr;
    size_t len_dat_, num_read_;
    FILE* input_ = nullptr;

private:
    bool fill();
    bool readOriChunk(char* data, size_t& length);
    bool readChunk(char* data, size_t& length);

public:
    LZ4In();
    LZ4In(const char* file_name) : LZ4In() {
        input_ = fopen(file_name, "rb");
        if (input_ == nullptr) {
            fprintf(stderr, "Open file '%s' failed!\n", file_name);
            exit(1);
        }
    }
    virtual ~LZ4In();

    // disalbe copy constructor
    LZ4In(const LZ4In&) = delete;

    // disable copy assignment
    LZ4In& operator=(const LZ4In&) = delete;

    // move constructor
    LZ4In(LZ4In&& other)
        : chunk_buf_(std::move(other.chunk_buf_)),
          data_buf_(std::move(other.data_buf_)),
          len_dat_(std::move(other.len_dat_)),
          num_read_(std::move(other.num_read_)),
          input_(std::move(other.input_)) {
        other.chunk_buf_ = other.data_buf_ = nullptr;
        other.input_ = nullptr;
        other.len_dat_ = other.num_read_ = 0;
    }

    // move assignment
    LZ4In& operator=(LZ4In&& other) {
        chunk_buf_ = std::move(other.chunk_buf_);
        data_buf_ = std::move(other.data_buf_);
        len_dat_ = std::move(other.len_dat_);
        num_read_ = std::move(other.num_read_);
        input_ = std::move(other.input_);
        other.chunk_buf_ = other.data_buf_ = nullptr;
        other.input_ = nullptr;
        other.len_dat_ = other.num_read_ = 0;
        return *this;
    }

    void open(const char* file_name);
    void close() override;
    bool eof() override;

    /**
     * Read characters of length at most len bytes. The returned string placed
     * in 'data' is *not* null-terminated, and is of length at most 'len'. The
     * function return value is the actual number of bytes placed in buffer.
     */
    size_t read(const void* data, const size_t len) override;

    /**
     * Read characters until a newline ('\n') is encountered. If a newline
     * character is not encountered in the first (len - 1) bytes, then the
     * excess characters are discarded. The returned string placed in 'data' is
     * null-terminated and includes the newline character if it was read in the
     * first (n - 1) bytes. The function return value is the number of bytes
     * placed in buffer (which includes the newline character if encountered,
     * but excludes the terminating null byte).
     **/
    size_t readLine(const void* data, const size_t len) override;

    void decompress(const char* output_file_name);
};
}

#endif /* __LZ4IO_H__ */
