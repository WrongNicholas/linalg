#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <initializer_list>
#include <vector>
#include <span>
#include <iostream>
#include <stdexcept>

// A Matrix represented as a one-dimensional vector, column-major.
template <typename T>
class Matrix {
private:
  size_t _rows;
  size_t _cols;
  std::vector<T> _data;
public:
  // Constructors
  Matrix(const size_t rows, const size_t cols);
  Matrix(std::size_t rows, std::size_t cols, std::initializer_list<T> initializer);
  // ** ADD CONSTRUCTOR FROM VEC OF VEC AND initializer_list of VEC

  // Accessors
  size_t rows() const noexcept { return _rows; }
  size_t cols() const noexcept { return _cols; }

  // Operator overloads
  T& operator()(size_t r, size_t c);
  const T& operator()(size_t r, size_t c) const;

  // Public helpers
  inline T& at(size_t r, size_t c);
  inline const T& at(size_t r, size_t c) const;

  /**
   * @brief Returns a span representing the specified column.
   *
   * @param c The zero-based index of the column.
   * @return A std::span<T> providing access to the elements of the column.
   *
   * @throws std::out_of_range If the column index is outside the valid range.
   */
  std::span<T> col_at(size_t c);

  /**
   * @brief Returns a const span representing the specified column.
   *
   * @param c The zero-based index of the column.
   * @return A std::span<const T> providing access to the elements of the column.
   *
   * @throws std::out_of_range If the column index is outside the valid range.
   */
  std::span<const T> col_at(size_t c) const;

  // std::vector<T> row_at(size_t r);

  // Debugging Utility
  /** 
   * @brief
   */
  void print() const;
  void print_col(size_t c) const;
};

// Constructors
template <typename T>
Matrix<T>::Matrix(size_t rows, size_t cols) : _rows(rows), _cols(cols), _data(rows * cols) 
{
  if (rows == 0 || cols == 0) throw std::invalid_argument("Matrix dimensions cannot be zero.");
}

template <typename T>
Matrix<T>::Matrix(std::size_t rows, std::size_t cols, std::initializer_list<T> initializer) : _rows(rows), _cols(cols), _data(initializer) 
{ 
  if (rows == 0 || cols == 0) throw std::invalid_argument("Matrix dimensions cannot be zero.");
  if (initializer.size() != rows * cols) throw std::invalid_argument("Initializer list does not match matrix dimensions.");
}

// Operator Overloads
template <typename T>
T& Matrix<T>::operator()(size_t r, size_t c)
{
  return at(r,c);
}

template <typename T>
const T& Matrix<T>::operator()(size_t r, size_t c) const
{
  return at(r,c);
}

// Helpers
template <typename T>
inline T& Matrix<T>::at(size_t r, size_t c)
{
  if (r >= _rows  || c >= _cols ) throw std::out_of_range("Requested position outside of matrix dimensions.");
  return _data[c * _rows + r];
}

template <typename T>
inline const T& Matrix<T>::at(size_t r, size_t c) const
{
  if (r >= _rows  || c >= _cols ) throw std::out_of_range("Requested position outside of matrix dimensions.");
  return _data[c * _rows + r];
}

template <typename T>
std::span<T> Matrix<T>::col_at(size_t c)
{
  if (c >= _cols ) throw std::out_of_range("Requested position outside of matrix dimensions.");
  return std::span(&_data[c * _rows], _rows);
}

template <typename T>
std::span<const T> Matrix<T>::col_at(size_t c) const
{
  if (c >= _cols ) throw std::out_of_range("Requested position outside of matrix dimensions.");
  return std::span(&_data[c * _rows], _rows);
}

template <typename T>
void Matrix<T>::print() const
{
  for (std::size_t r = 0; r < _rows; ++r) {
      for (std::size_t c = 0; c < _cols; ++c) {
          std::cout << at(r, c);
          if (c + 1 < _cols) std::cout << ", ";
      }
      std::cout << '\n';
  }
}

template <typename T>
void Matrix<T>::print_col(size_t c) const
{
  for (const T& element : col_at(c))
  {
    std::cout << element << std::endl;
  }
}

#endif
