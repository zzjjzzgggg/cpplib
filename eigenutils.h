#ifndef __EIGENUTILS_H__
#define __EIGENUTILS_H__

#include <fstream>
#include <string>

namespace eigenutils {

template <class Matrix>
void write(const std::string &filename, const Matrix &matrix) {
  std::ofstream out(filename, std::ios::out | std::ios::binary |
                                  std::ios::trunc);
  typename Matrix::Index rows = matrix.rows(),
                         cols = matrix.cols();
  out.write((char *)(&rows), sizeof(typename Matrix::Index));
  out.write((char *)(&cols), sizeof(typename Matrix::Index));
  out.write((char *)matrix.data(),
            rows * cols * sizeof(typename Matrix::Scalar));
  out.close();
}

template <class Matrix>
void read(const std::string &filename, Matrix &matrix) {
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  typename Matrix::Index rows = 0, cols = 0;
  in.read((char *)(&rows), sizeof(typename Matrix::Index));
  in.read((char *)(&cols), sizeof(typename Matrix::Index));
  matrix.resize(rows, cols);
  in.read((char *)matrix.data(),
          rows * cols * sizeof(typename Matrix::Scalar));
  in.close();
}

template <class Tensor>
void write_tensor(const std::string &filename, const Tensor &tensor) {
  std::ofstream out(filename, std::ios::out | std::ios::binary |
                                  std::ios::trunc);
  typename Tensor::Index rank = tensor.rank();
  out.write((char *)(&rank), sizeof(typename Tensor::Index));
  out.write(
      (char *)tensor.dimensions().data(),
      rank * sizeof(typename Tensor::Index));
  out.write((char *)tensor.data(),
            tensor.size() * sizeof(typename Tensor::Scalar));
  out.close();
}

template <class Tensor>
void read_tensor(const std::string &filename, Tensor &tensor) {
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  typename Tensor::Index rank;
  in.read((char *)(&rank), sizeof(typename Tensor::Index));
  typename Tensor::Dimensions dimensions;
  in.read((char *)dimensions.data(),
          rank * sizeof(typename Tensor::Index));
  tensor.resize(dimensions);
  in.read((char *)tensor.data(),
          tensor.size() * sizeof(typename Tensor::Scalar));
  in.close();
}
}

#endif /* __EIGENUTILS_H__ */
