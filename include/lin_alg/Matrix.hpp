#pragma once

#ifndef WOJI_MATRIX_HPP
#define WOJI_MATRIX_HPP

#include <initializer_list>
#include <vector>
#include <span>
#include <iostream>
#include <stdexcept>

/**
 * @brief A column-major matrix stored internaly as a one-dimensional std::vector.
 *
 * @tparam T Element type stored in the matrix.
 *
 * This class stores elements in a column-major order, meaning elements of the same
 * column are stored contiguously. The element at row `r` and column `c` is located
 * at index `c * rows() + r` in the underlying vector.
 */
template <typename T>
class Matrix {
private:
  /** Number of rows in the matrix. */
  size_t _rows;

  /** Number of columns in the matrix. */
  size_t _cols;

  /** Contiguous storage in column-major order. */
  std::vector<T> _data;

public:
  // ==============================================================================
  // Constructors
  // ==============================================================================

  /**
   * @brief Constructs a matrix with the given dimensions.
   *
   * @param rows Number of rows.
   * @param cols Number of columns.
   *
   * @throws std::invalid_argument If matrix dimensions are zero.
   *
   * @note All matrix entries will be default-initialized.
   */
  Matrix(const size_t rows, const size_t cols);

  /** 
   * @brief Constructs a matrix with the given dimensions and initializer list.
   *
   * @param rows Number of rows.
   * @param cols Number of columns.
   * @param initializer Elements to fill the matrix with, in column-major order.
   *
   * @throws std::invalid_argument If matrix dimensions are zero or @p initializer
   * is not of size `rows * cols`.
   */
  Matrix(std::size_t rows, std::size_t cols, std::initializer_list<T> initializer);

  /** 
   * @brief Constructs a matrix with the given initializer list of columns.
   *
   * @param initializer A list of columns.
   * @throws std::invalid_argument If the matrix would have zero rows or zero columns,
   * or if the columns have different lengths.
   */
  Matrix(std::initializer_list<std::initializer_list<T>> initializer);

  /**
   * @brief Constructs a new matrix as a copy of another matrix.
   *
   * @param other The matrix to copy from.
   *
   * @note Performs a deep copy of all elements, preserving the original matrix dimensions.
   *
   */
  Matrix(const Matrix<T>& other);

  // ==============================================================================
  // Accessors
  // ==============================================================================
  
  /** Returns the number of rows. */
  size_t rows() const noexcept { return _rows; }
  /** Returns the number of columns. */
  size_t cols() const noexcept { return _cols; }
  /** Returns a mutable reference to the underlying data vector of the matrix. */
  std::vector<T>& data() noexcept { return _data; }
  /** Returns a const reference to the underlying data vector of the matrix. */
  const std::vector<T>& data() const noexcept { return _data; }

  // ==============================================================================
  // Operator Overloads
  // ==============================================================================
  
  /**
   * @brief Multiplies this matrix by another matrix and returns the result.
   *
   * @param other The matrix to multiply with this matrix.
   * @returns A new Matrix<T> containing the product of the two matrices.
   *
   * @throws std::invalid_argument If the matrices have incompatible sizes.
   */
  Matrix<T> operator*(const Matrix<T>& other) const;

  /**
   * @brief Multiplies this matrix by a scalar and returns the result.
   *
   * @param scalar The scalar to multiply with this matrix.
   * @returns A new Matrix<T> containing the product of this matrix and the scalar.
   *
   */
  Matrix<T> operator*(const T& scalar) const;

  /**
   * @brief Adds this matrix to another matrix and returns the result.
   *
   * @param other The matrix to add to this matrix.
   * @return A new Matrix<T> representing the element-wise sum of the two matrices.
   *
   * @throws std::invalid_argument If the matrices have different dimensions.
   *
   */
  Matrix<T> operator+(const Matrix<T>& other) const;

  /**
   * @brief Checks whether this matrix is equal to another matrix.
   *
   * @param other The matrix to compare with this matrix.
   * @returns True if both matrices are equal. False otherwise.
   */
  bool operator==(const Matrix<T>& other) const;

  /**
   * @brief Checks whether this matrix is not equal to another matrix.
   *
   * @param other The matrix to compare with this matrix.
   * @returns True if both matrices are not equal. False otherwise.
   */
  bool operator!=(const Matrix<T>& other) const;

  // ==============================================================================
  // Element access
  // ==============================================================================
  
  /**
   * @brief Returns a reference to the element at (r, c).
   *
   * @param r Zero-based row index.
   * @param c Zero-based column index.
   * @return Reference to the element at (r, c).
   *
   * @throws std::out_of_range If @p r or @p c is outside the valid range.
   */
  T& operator()(size_t r, size_t c);

  /**
   * @brief Returns a const reference to the element at (r, c).
   *
   * @param r Zero-based row index.
   * @param c Zero-based column index.
   * @return Const reference to the element at (r, c).
   *
   * @throws std::out_of_range If @p r or @p c is outside the valid range.
   */
  const T& operator()(size_t r, size_t c) const;

  /**
   * @brief Returns a reference to the element at (r, c).
   *
   * @param r Zero-based row index.
   * @param c Zero-based column index.
   * @return Reference to the element at (r, c).
   *
   * @throws std::out_of_range If @p r or @p c is outside the valid range.
   */
  T& at(size_t r, size_t c);

  /**
   * @brief Returns a const reference to the element at (r, c).
   *
   * @param r Zero-based row index.
   * @param c Zero-based column index.
   * @return Const reference to the element at (r, c).
   *
   * @throws std::out_of_range If @p r or @p c is outside the valid range.
   */
  const T& at(size_t r, size_t c) const;

  // ==============================================================================
  // Column Access
  // ==============================================================================

  /**
   * @brief Returns a span representing the specified column.
   *
   * @param c The zero-based index of the column.
   * @return A std::span<T> providing access to the elements of the column.
   *
   * @throws std::out_of_range If @p c is outside the valid range.
   *
   * @note The returned span is a direct view into the underlying storage. Modifying
   * the span will modify the corresponding entries in the matrix.
   */
  std::span<T> col_at(size_t c);

  /**
   * @brief Returns a const span representing the specified column.
   *
   * @param c The zero-based index of the column.
   * @return A std::span<const T> providing read-only access to the elements of the column.
   *
   * @throws std::out_of_range If the column index is outside the valid range.
   */
  std::span<const T> col_at(size_t c) const;

  // ==============================================================================
  // Printing Utility
  // ==============================================================================

  /** Prints the matrix to the standard output stream in row-major format. */
  void print_row_major() const;
  /**
   * @brief Prints a single column to the standard output stream.
   *
   * @param c Zero-based column index to print.
   *
   * @throws std::out_of_range If @p c is outside the valid range.
   */
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

template <typename T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> initializer)
{
  _cols = initializer.size();
  if (_cols == 0) throw std::invalid_argument("Matrix must have at least one column.");
  _rows = initializer.begin()->size();
  if (_rows == 0) throw std::invalid_argument("Matrix columns cannot be empty.");

  for (const auto& col : initializer) {
    if (col.size() != _rows) throw std::invalid_argument("All columns must have the same number of columns.");
  }

  _data.reserve(_rows * _cols);
  for (const auto& col : initializer) {
    _data.insert(_data.end(), col.begin(), col.end());
  }
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& other) : _rows(other.rows()), _cols(other.cols()), _data(other.data())
{

}

// Operator Overloads
template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) const
{
  if (cols() != other.rows()) throw std::invalid_argument("Matrix sizes are mismatched!");

  // (AB)_ij = a_i1*b_1j + a_i2*b_2j + ... + a_in*b_nj
  // i.e. (AB)_ij = summation from k = 0 -> n - 1 (A_ik * B_kj)
  Matrix<T> product = Matrix<T>(rows(), other.cols());

  for (size_t i = 0; i < rows(); ++i)
  {
    for (size_t j = 0; j < other.cols(); ++j)
    {
      T sum{};
      for (size_t k = 0; k < cols(); ++k)
      {
        sum += at(i, k) * other.at(k, j);
      }
      product(i, j) = sum;
    }
  }
  return product;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const T& scalar) const
{
  Matrix<T> product = Matrix<T>(*this);
  

  for (auto it = product._data.begin(); it != product._data.end(); ++it)
  {
    (*it) *= scalar;
  }

  return product;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& other) const
{
  if (rows() != other.rows() || cols() != other.cols()) throw std::invalid_argument("Matrix sizes are mismatched!");
  Matrix<T> sum = Matrix<T>(rows(), cols());

  auto sum_it = sum.data().begin();
  auto lhs_it = data().cbegin();
  auto lhs_end = data().cend();
  auto other_it = other.data().cbegin();
  for (; lhs_it != lhs_end; ++lhs_it, ++other_it, ++sum_it)
  {
    *sum_it = *lhs_it + *other_it;
  }

  return sum;
}

template <typename T>
bool Matrix<T>::operator==(const Matrix<T>& other) const
{
  return data() == other.data();
}

template <typename T>
bool Matrix<T>::operator!=(const Matrix<T>& other) const
{
  return data() != other.data();
}

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
void Matrix<T>::print_row_major() const
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
