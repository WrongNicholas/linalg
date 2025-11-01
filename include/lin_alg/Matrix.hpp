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
 *
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

  // ==============================================================================
  // Accessors
  // ==============================================================================
  
  /** Returns the number of rows. */
  size_t rows() const noexcept { return _rows; }
  /** Returns the number of columns. */
  size_t cols() const noexcept { return _cols; }

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
  inline T& at(size_t r, size_t c);

  /**
   * @brief Returns a const reference to the element at (r, c).
   *
   * @param r Zero-based row index.
   * @param c Zero-based column index.
   * @return Const reference to the element at (r, c).
   *
   * @throws std::out_of_range If @p r or @p c is outside the valid range.
   */
  inline const T& at(size_t r, size_t c) const;

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
