#ifndef __LZ4IO_H__
#define __LZ4IO_H__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <algorithm>

#include <lz4.h>

namespace lz4 {

enum {
  BLOCK_BYTES = 65536,  // 64KB
};

static const size_t DATA_CAPACITY = BLOCK_BYTES;
static const size_t CHUNK_CAPACITY =
    LZ4_COMPRESSBOUND(BLOCK_BYTES);

class LZ4Out {
private:
  char *data_buf_, *chunk_buf_;
  size_t len_dat_;
  FILE* output_;

private:
  void writeChunk();

public:
  LZ4Out();
  LZ4Out(const char* file_name, const bool append = false)
      : LZ4Out() {
    open(file_name, append);
  }

  // do not allow copy constructor or assignment
  LZ4Out(const LZ4Out&) = delete;
  LZ4Out& operator=(const LZ4Out&) = delete;

  // move constructor
  LZ4Out(LZ4Out&& other)
      : data_buf_(std::move(other.data_buf_)),
        chunk_buf_(std::move(other.chunk_buf_)),
        len_dat_(other.len_dat_),
        output_(std::move(other.output_)) {
    other.data_buf_ = other.chunk_buf_ = NULL;
    other.len_dat_ = 0;
    other.output_ = NULL;
  }
  // move assignment
  LZ4Out& operator=(LZ4Out&& other) {
    data_buf_ = std::move(other.data_buf_);
    chunk_buf_ = std::move(other.chunk_buf_);
    len_dat_ = other.len_dat_;
    output_ = std::move(other.output_);
    other.data_buf_ = other.chunk_buf_ = NULL;
    other.len_dat_ = 0;
    other.output_ = NULL;
    return *this;
  }

  ~LZ4Out();

  void open(const char* file_name, const bool append = false);
  // add data to buffer, the data may be larger than buffer
  // size.
  void write(const void* data, const size_t length);
  void save(const int val) { write(&val, sizeof(int)); }
  void save(const long val) { write(&val, sizeof(long)); }
  void save(const double val) {
    write(&val, sizeof(double));
  };
  void close();
  bool isClosed() const { return output_ == NULL; }

  void compress(const char* input_file_name);
};

class LZ4In {
private:
  char *chunk_buf_, *data_buf_;
  size_t len_data_, num_read_;
  FILE* input_;

private:
  bool fillBuffer();
  bool readOriChunk(char* data, size_t& length);
  bool readChunk(char* data, size_t& length);

public:
  LZ4In();
  LZ4In(const char* file_name) : LZ4In() {
    input_ = fopen(file_name, "rb");
    if (input_ == NULL) {
      fprintf(stderr, "Open file '%s' failed!\n", file_name);
      exit(1);
    }
  }
  ~LZ4In();

  // do not allow copy constructor or assignment
  LZ4In(const LZ4In&) = delete;
  LZ4In& operator=(const LZ4In&) = delete;

  // move constructor
  LZ4In(LZ4In&& other)
      : chunk_buf_(other.chunk_buf_),
        data_buf_(other.data_buf_),
        len_data_(other.len_data_),
        num_read_(other.num_read_), input_(other.input_) {
    other.chunk_buf_ = other.data_buf_ = NULL;
    other.len_data_ = other.num_read_ = 0;
    other.input_ = NULL;
  }

  void open(const char* file_name);
  void close();
  bool eof();

  size_t read(const void* data, const size_t len);
  void load(int& val) { read(&val, sizeof(int)); }
  void load(long& val) { read(&val, sizeof(long)); }
  void load(double& val) { read(&val, sizeof(double)); }

  void decompress(const char* output_file_name);
};
}

#endif /* __LZ4IO_H__ */
