#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <initializer_list>
#include <vector>
#include <span>
#include <iostream>

// A Matrix represented as a one-dimensional vector, column-major.
// Can be accessed row-major through helper methods.
// REQUIRES PROPER INITIALIZATION! (might need to add some sort of throw or check for initialization)
template <typename T>
class Matrix {
private:
  const size_t rows;   // Number of rows in the matrix
  const size_t cols;   // Number of columns in the matrix
  std::vector<T> data; // Matrix stored as a one-dimensional vector
public:
  // Constructors
  Matrix(const size_t rown, const size_t cols);                                     // Constructs an 'empty'  m * n matrix
  Matrix(std::size_t rows, std::size_t cols, std::initializer_list<T> initializer); // Constructs an m * n matrix using an initializer list
  // ** ADD CONSTRUCTOR FROM VEC OF VEC AND initializer_list of VEC

  // Public helpers
  // need const/nonconst, also figure out required refs.
  std::span<T> col_at(int j);                                                       // returns a span of a specific col
  std::vector<T> row_at(size_t i);                                                  // returns a vector of a specific row

  void print() const;                                                               // Prints rows
};

template <typename T>
Matrix<T>::Matrix(size_t rows, size_t cols) : rows(rows), cols(cols), data(rows * cols) { }

template <typename T>
Matrix<T>::Matrix(std::size_t rows, std::size_t cols, std::initializer_list<T> initializer) : rows(rows), cols(cols), data(initializer) { }

template <typename T>
std::span<T> Matrix<T>::col_at(int col)
{
  return std::span(&data[col * rows], rows);
}

template <typename T>
std::vector<T> Matrix<T>::row_at(size_t row)
{
  std::vector<T> vec;
  vec.reserve(this->cols);
  for (size_t col = 0; col < this->cols; ++col)
  {
    vec.push_back(this->data[col * this->rows + row]);
  }

  return vec;
}

template <typename T>
void Matrix<T>::print() const {
    for (std::size_t r = 0; r < rows; ++r) {
        for (std::size_t c = 0; c < cols; ++c) {
            std::cout << data[c * rows + r];
            if (c + 1 < cols) std::cout << ", ";
        }
        std::cout << '\n';
    }
}

#endif
