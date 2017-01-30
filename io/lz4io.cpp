#include "lz4io.h"

namespace ioutils {

size_t writeInt(FILE* fp, const int i) {
  return std::fwrite(&i, sizeof(int), 1, fp);
}

size_t writeBin(FILE* fp, const void* array,
                const size_t arrayBytes) {
  return std::fwrite(array, 1, arrayBytes, fp);
}

size_t readInt(FILE* fp, int* i) {
  return std::fread(i, sizeof(int), 1, fp);
}

size_t readBin(FILE* fp, void* array,
               const size_t arrayBytes) {
  return std::fread(array, 1, arrayBytes, fp);
}

//============================================================

LZ4Out::LZ4Out() {
  data_buf_ = new char[DATA_CAPACITY];
  chunk_buf_ = new char[CHUNK_CAPACITY];
  if (data_buf_ == nullptr || chunk_buf_ == nullptr) {
    std::fprintf(stderr, "Allocate space failed!\n");
    exit(1);
  }
  len_dat_ = 0;
  output_ = nullptr;
}

LZ4Out::~LZ4Out() {
  close();
  if (data_buf_ != nullptr) delete[] data_buf_;
  if (chunk_buf_ != nullptr) delete[] chunk_buf_;
}

void LZ4Out::close() {
  if (output_ != nullptr) {
    writeChunk();
    fclose(output_);
    output_ = nullptr;
  }
}

void LZ4Out::open(const char* file_name, const bool append) {
  close();
  const char* mode = append ? "ab" : "wb";
  output_ = std::fopen(file_name, mode);
  if (output_ == nullptr) {
    std::fprintf(stderr, "Open file '%s' failed!\n",
                 file_name);
    exit(1);
  }
}

void LZ4Out::write(const void* data, const size_t len) {
  size_t written = 0;
  while (len - written > 0) {
    size_t num_available = DATA_CAPACITY - len_dat_;
    if (num_available == 0) writeChunk();
    size_t num_to_write =
        std::min(len - written, num_available);
    std::memcpy(data_buf_ + len_dat_, (char*)data + written,
                num_to_write);
    len_dat_ += num_to_write;
    written += num_to_write;
  }
}

void LZ4Out::writeChunk() {
  if (len_dat_ == 0) return;
  size_t chunk_len = LZ4_compress_fast(
      data_buf_, chunk_buf_, len_dat_, CHUNK_CAPACITY, 9);
  if (chunk_len > 0) {
    writeInt(output_, chunk_len);
    writeBin(output_, chunk_buf_, chunk_len);
  }
  len_dat_ = 0;
}

void LZ4Out::compress(const char* file_name) {
  FILE* file_id = std::fopen(file_name, "rb");
  if (file_id == nullptr) {
    std::fprintf(stderr, "Open file '%s' failed!\n",
                 file_name);
    exit(1);
  }
  char buffer[1024];
  size_t num_read;
  while (true) {
    num_read = fread(buffer, 1, 1024, file_id);
    write(buffer, num_read);
    if (num_read < 1024) break;
  }
  fclose(file_id);
  close();
}

//=================================================

// const std::unordered_set<std::string> LZ4In::lz4_ext_set{
//     {".lz", ".lz4"}};

LZ4In::LZ4In() {
  chunk_buf_ = new char[CHUNK_CAPACITY];
  data_buf_ = new char[DATA_CAPACITY];
  if (data_buf_ == nullptr || chunk_buf_ == nullptr) {
    std::fprintf(stderr, "Allocate space failed!\n");
    exit(1);
  }
  input_ = nullptr;
  len_data_ = num_read_ = 0;
}

LZ4In::~LZ4In() {
  close();
  if (chunk_buf_ != nullptr) delete[] chunk_buf_;
  if (data_buf_ != nullptr) delete[] data_buf_;
}

void LZ4In::close() {
  if (input_ != nullptr) {
    fclose(input_);
    input_ = nullptr;
  }
  len_data_ = num_read_ = 0;
}

void LZ4In::open(const char* file_name) {
  close();
  input_ = std::fopen(file_name, "rb");
  if (input_ == nullptr) {
    std::fprintf(stderr, "Open file '%s' failed!\n",
                 file_name);
    exit(1);
  }
}

bool LZ4In::readOriChunk(char* data, size_t& len) {
  len = 0;
  int chunk_len;
  size_t num_read = readInt(input_, &chunk_len);
  if (num_read < 1 || chunk_len == 0) return false;
  len = readBin(input_, data, chunk_len);
  return len > 0;
}

bool LZ4In::readChunk(char* data, size_t& len) {
  len = 0;
  int chunk_len;
  size_t num_read = readInt(input_, &chunk_len);
  if (num_read < 1 || chunk_len == 0) return false;
  readBin(input_, chunk_buf_, chunk_len);
  len = LZ4_decompress_safe(chunk_buf_, data, chunk_len,
                            (int)DATA_CAPACITY);
  return len > 0;
}

bool LZ4In::eof() {
  if (num_read_ < len_data_) return false;
  return !fillBuffer();
}

bool LZ4In::fillBuffer() {
  num_read_ = 0;
  return readChunk(data_buf_, len_data_);
}

size_t LZ4In::read(const void* data, const size_t len) {
  if (len <= 0 || data == nullptr) return 0;
  size_t num_have_read = 0, num_to_read = 0;
  while (!eof() && num_have_read < len) {
    // min(num_requirement, num_available)
    num_to_read =
        std::min(len - num_have_read, len_data_ - num_read_);
    std::memcpy((char*)data + num_have_read,
                data_buf_ + num_read_, num_to_read);
    num_read_ += num_to_read;
    num_have_read += num_to_read;
  }
  return num_have_read;
}

size_t LZ4In::readLine(const void* data, const size_t len) {
  if (len <= 0 || data == nullptr) return 0;
  size_t num_have_read = 0, num_to_read = 0;
  while (!eof() && num_have_read < len - 1) {
    // min(num_requirement, num_available)
    num_to_read =
        std::min(len - num_have_read, len_data_ - num_read_);
    char* src_ptr = data_buf_ + num_read_;
    char* dst_ptr = (char*)data + num_have_read;
    for (size_t i = 0; i < num_to_read; i++) {
      *dst_ptr++ = *src_ptr++;
      num_have_read++;
      num_read_++;
      if (*(dst_ptr - 1) == '\n') break;
    }
    if (*(dst_ptr - 1) == '\n') break;
  }
  *((char*)data + num_have_read) = '\0';
  return num_have_read;
}

void LZ4In::decompress(const char* output_file_name) {
  FILE* file_id = std::fopen(output_file_name, "wb");
  char buffer[1024];
  size_t num_read;
  while (!eof()) {
    num_read = read(buffer, 1024);
    writeBin(file_id, buffer, num_read);
  }
  fclose(file_id);
}
}
