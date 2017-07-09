#include "gzipio.h"

namespace ioutils {

const size_t GZipOut::MAX_BUF_SIZE = 4 * 1024;

GZipOut::GZipOut(const std::string& filename) : buf_size_(0) {
    zip_wr_ = popen(getCmd(filename).c_str(), "w");
    buf_ = new char[MAX_BUF_SIZE];
}

GZipOut::~GZipOut() {
    close();
    if (buf_ != NULL) delete[] buf_;
}

void GZipOut::flush() {
    fwrite(buf_, 1, buf_size_, zip_wr_);
    buf_size_ = 0;
}

void GZipOut::close() {
    if (buf_size_ != 0) flush();
    if (zip_wr_ != NULL) {
        pclose(zip_wr_);
        zip_wr_ = NULL;
    }
}

void GZipOut::write(const void* dat, const size_t len) {
    char* src = (char*)dat;
    for (size_t i = 0; i < len; i++) {
        if (buf_size_ < MAX_BUF_SIZE)
            buf_[buf_size_++] = src[i];
        else
            flush();
    }
}

std::string GZipOut::getCmd(const std::string& filename) {
    std::string base, name, ext;
    strutils::splitFilename(filename, base, name, ext);
    return fmt::format("7za a -y -bd -si{} {} > /dev/null", name.c_str(),
                       filename.c_str());
}

// GZipIn
const size_t GZipIn::MAX_BUF_SIZE = 32 * 1024;

GZipIn::GZipIn(const std::string& filename) {
    zip_rd_ = popen(getCmd(filename).c_str(), "r");
    buf_ = new char[MAX_BUF_SIZE];
    fill();
}

GZipIn::~GZipIn() {
    if (zip_rd_ != NULL) pclose(zip_rd_);
    if (buf_ != NULL) delete[] buf_;
}

bool GZipIn::eof() {
    if (num_read_ < buf_size_) return false;
    return !fill();
}

bool GZipIn::fill() {
    buf_size_ = fread(buf_, 1, MAX_BUF_SIZE, zip_rd_);
    num_read_ = 0;
    return buf_size_ > 0;
}

size_t GZipIn::read(const void* dat, const size_t len) {
    if (len <= 0 || dat == nullptr) return 0;
    size_t num_read = 0, num_to_read = 0;
    while (!eof() && num_read < len) {
        num_to_read = std::min(len - num_read, buf_size_ - num_read_);
        std::memcpy((char*)dat + num_read, buf_ + num_read_, num_to_read);
        num_read += num_to_read;
        num_read_ += num_to_read;
    }
    return num_read;
}

size_t GZipIn::readLine(const void* dat, const size_t len) {
    size_t num_read = 0, num_to_read;
    while (!eof() && num_read < len - 1) {
        num_to_read = std::min(len - num_read, buf_size_ - num_read_);
        char* src_ptr = buf_ + num_read_;
        char* dst_ptr = (char*)dat + num_read;
        for (size_t i = 0; i < num_to_read; i++) {
            *dst_ptr++ = *src_ptr++;
            num_read++;
            num_read_++;
            if (*(dst_ptr - 1) == '\n') break;
        }
        if (*(dst_ptr - 1) == '\n') break;
    }
    *((char*)dat + num_read) = '\0';
    return num_read;
}

std::string GZipIn::getCmd(const std::string& filename) const {
    return fmt::format("7za e -y -bd -so {} 2> /dev/null", filename.c_str());
}

}  // end namespace ioutils
