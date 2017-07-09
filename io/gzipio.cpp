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

void GZipIn::fill() {
    buf_size_ = fread(buf_, 1, MAX_BUF_SIZE, zip_rd_);
    cur_pos_ = 0;
}

size_t GZipIn::read(const void* dat, const size_t len) {
    char* sink = (char*)dat;
    size_t num_read = 0;
    for (size_t i = 0; i < len; i++) {
        if (cur_pos_ == buf_size_) {
            fill();
            if (eof()) break;  // no more data to read
        }
        sink[i] = buf_[cur_pos_++];
        num_read++;
    }
    return num_read;
}

size_t GZipIn::readLine(const void* dat, const size_t len) {
    char* sink = (char*)dat;
    size_t num_read = 0;
    while (num_read < len - 1) {
        if (cur_pos_ == buf_size_) {
            fill();
            if (eof()) break;
        }
        *sink = buf_[cur_pos_++];
        num_read++;
        if (*sink == '\n') break;
        sink++;
    }
    *((char*)dat + num_read) = '\0';
    return num_read;
}

std::string GZipIn::getCmd(const std::string& filename) const {
    return fmt::format("7za e -y -bd -so {} 2> /dev/null", filename.c_str());
}

}  // end namespace ioutils
