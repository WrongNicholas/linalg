#pragma once

#ifndef WOJI_MATRIX_HPP
#define WOJI_MATRIX_HPP

#include <algorithm>
#include <initializer_list>
#include <vector>
#include <span>
#include <iostream>
#include <stdexcept>

/**
 * @brief A row-major matrix stored internally as a one-dimensional std::vector.
 *
 * @tparam T Element type stored in the matrix.
 *
 * This class stores elements in a row-major order, meaning elements of the same
 * row are stored contiguously. The element at row `r` and column `c` is located
 * at index `r * cols() + c` in the underlying vector.
 */
template <typename T>
class Matrix {
private:
  /** Number of rows in the matrix. */
  size_t _rows;

  /** Number of columns in the matrix. */
  size_t _cols;

  /** Contiguous storage in row-major order. */
  std::vector<T> _data;


public:
  /**
   * @brief Represents the result of a Reduced Row Echelon Form operation.
   *
   * Contains both the transformed matrix and additional data needed to compute 
   * a determinant.
   *
   * @tparam T Element type of the matrix.
   */
  struct RrefResult {
    Matrix<T> m;
    size_t swaps = 0;
    T scale_prod = T{1};
  };

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
  explicit Matrix(const size_t rows, const size_t cols);

  /** 
   * @brief Constructs a matrix with the given dimensions and initializer list.
   *
   * @param rows Number of rows.
   * @param cols Number of columns.
   * @param initializer Elements to fill the matrix with, in row-major order.
   *
   * @throws std::invalid_argument If matrix dimensions are zero or @p initializer
   * is not of size `rows * cols`.
   */
  Matrix(std::size_t rows, std::size_t cols, std::initializer_list<T> initializer);

  /** 
   * @brief Constructs a matrix with the given initializer list of rows.
   *
   * @param initializer A list of rows.
   * @throws std::invalid_argument If the matrix would have zero rows or zero columns,
   * or if the rows have different lengths.
   */
  Matrix(std::initializer_list<std::initializer_list<T>> initializer);

  /** 
   * @brief Constructs a matrix with the given initializer list of columns.
   *
   * @param initializer A list of columns.
   * @throws std::invalid_argument If the matrix would have zero rows or zero columns,
   * or if the columns have different lengths.
   */
  static Matrix<T> from_columns(std::initializer_list<std::initializer_list<T>> columns)
  {
    size_t cols = columns.size();
    if (cols == 0) throw std::invalid_argument("Matrix must have at least one column.");

    size_t rows = columns.begin()->size();
    if (rows == 0) throw std::invalid_argument("Matrix columns cannot be empty.");

    for (const auto& col : columns)
    {
      if (col.size() != rows) throw std::invalid_argument("All columns must have the same number of rows.");
    }

    Matrix<T> m(rows, cols);

    size_t c = 0;
    for (const auto& col : columns)
    {
      size_t r = 0;
      for (const auto& element : col)
      {
        m.at(r, c) = element;
        ++r;
      }
      ++c;
    }

    return m;
  }

  /**
   * @brief Constructs a new matrix as a copy of another matrix.
   *
   * @param other The matrix to copy from.
   *
   * @note Performs a deep copy of all elements, preserving the original matrix dimensions.
   *
   */
  Matrix(const Matrix<T>& other) = default;
  Matrix(Matrix&&) noexcept = default;
  Matrix& operator=(const Matrix&) = default;
  Matrix& operator=(Matrix&&) noexcept = default;

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
  // Arithmetic
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
   * @brief Multiplies every element of this matrix by a scalar in place.
   *
   * @param scalar The scalar value to multiply by.
   * @return Reference to this matrix after scaling.
   */
  Matrix<T>& operator*=(const T& scalar);

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
   * @brief Adds another matrix to this matrix in place.
   *
   * @param other The matrix to add.
   * @return Reference to this matrix after addition.
   *
   * @throws std::invalid_argument If the matricies have different dimensions.
   */
  Matrix<T>& operator+=(const Matrix<T>& other);

  // ==============================================================================
  // Operator Overloads
  // ==============================================================================
  
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
   *
   */
  std::span<T> operator[](int i);

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
  // Row Access
  // ==============================================================================

  /**
   * @brief Returns a span representing the specified row.
   *
   * @param r The zero-based index of the row.
   * @return A std::span<T> providing access to the elements of the row.
   *
   * @throws std::out_of_range If @p r is outside the valid range.
   *
   * @note The returned span is a direct view into the underlying storage. Modifying
   * the span will modify the corresponding entries in the matrix.
   */
  std::span<T> row_at(size_t r);

  /**
   * @brief Returns a const span representing the specified row.
   *
   * @param r The zero-based index of the row.
   * @return A std::span<const T> providing read-only access to the elements of the row.
   *
   * @throws std::out_of_range If the row index is outside the valid range.
   */
  std::span<const T> row_at(size_t r) const;

  // ==============================================================================
  // Row Operations
  // ==============================================================================
  /**
   * @brief Swaps two rows of the matrix in place.
   *
   * @param r1 Zero-based index of the first row to swap.
   * @param r2 Zero-based index of the second row to swap.
   *
   * @throws std::out_of_range If either @p r1 or @p r2 is outside the valid row range.
   */
  void swap_rows(size_t r1, size_t r2);

  /**
   * @brief Scales a row.
   *
   * @param r Zero-based index of the row to scale.
   * @param scalar Scalar value to multiply @p r by.
   *
   * @throws std::out_of_range If @p r is outside the valid row range.
   */
  void scale_row(size_t r, const T& scalar);

  /**
   * @brief Adds one row of the matrix, multiplied by a scalar to another row of the matrix.
   *
   * This operation performs: row[r2] += scalar * row[r1]
   *
   * @param r1 Zero-based index of the row to add to the other row.
   * @param r2 Zero-based index of the row to be added to.
   * @param scalar Scalar value to multiply @p r1 by before adding to @p r2.
   *
   * @throws std::out_of_range If @p r1 or @p r2 is outside the valid row range.
   */
  void add_row(size_t r1, size_t r2, const T& scalar);

  // ==============================================================================
  // Linear Algebra Operations
  // ==============================================================================

  /**
   * @brief Computes the Reduced Row Echelon Form (RREF) while recording values relating
   * to row operations.
   *
   * Similar to `rref()`, but returns additional information.
   *
   * @returns An `RrefResult<T>` struct, which contains:
   *  - `m`: The RREF of this matrix.
   *  - `swaps`: The number of row swaps performed.
   *  - `scale_prod`: The cumulative product of scaling factors.
   *
   *  @note This operation requires that the element type @p T supports:
   *  - Default initialization (i.e., @c T{} produces a valid zero value).
   *  - Equality comparison.
   *  - Arithmetic operations.
   *  - Constructions from literals (i.e., @c T{1}).
   *
   * @warning Usage of floating types such as @c double or @c float are recommended,
   * as division of integral types such as @c int may yield truncated results.
   *  
   */
  RrefResult rref_stats() const;

  /**
   * @brief Computes the Reduced Row Echelon Form (RREF) of the matrix.
   *
   * This operation performs a sequence of elementary row operations to reduce the
   * matrix into its canonical RREF form. Each pivot is normalized to 1 and used
   * to eliminate all nonzero elements in its column.
   *
   * @returns A new Matrix<T> containing the RREF of this matrix.
   *
   * @note This method requires that the element type @p T supports:
   *  - Default initialization (i.e., @c T{} produces a valid zero value).
   *  - Equality comparison.
   *  - Arithmetic operations.
   *  - Constructions from literals (i.e., @c T{1}).
   *
   * @warning Usage of floating types such as @c double or @c float are recommended,
   * as division of integral types such as @c int may yield truncated results.
   *
   * @snippet tests/test_matrix.cpp rref_example
   *
   * @see rref_stats()
   */
  Matrix<T> rref() const;

  /**
   * @brief Computes the determinant of a square matrix.
   *
   * @return The determinant of the matrix.
   *
   * @throws std::invalid_argument If the matrix is not square.
   *
   * @note The determinant is computed using Gaussian elimination through a RREF routine.
   *
   * @warning For best accuracy, use floating-point element types such as @c double or
   *          @c float. Integral element types may yield truncated due to integer division.
   *
   * @see rref_stats()
   */
  T det() const;

  // ==============================================================================
  // Printing Utility
  // ==============================================================================

  /** Prints the matrix to the standard output stream in row-major format. */
  void print() const;
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
  _rows = initializer.size();
  if (_rows == 0) throw std::invalid_argument("Matrix must have at least one row.");
  _cols = initializer.begin()->size();
  if (_cols == 0) throw std::invalid_argument("Matrix columns cannot be empty.");

  for (const auto& row : initializer) {
    if (row.size() != _cols) throw std::invalid_argument("All rows must have the same number of columns.");
  }

  _data.reserve(_rows * _cols);
  for (const auto& row : initializer) {
    _data.insert(_data.end(), row.begin(), row.end());
  }
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
        sum += _data[i * _cols + k] * other._data[k * other._cols + j];
      }
      product._data[i * product._cols + j] = sum;
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
Matrix<T>& Matrix<T>::operator*=(const T& scalar)
{
  for (auto& x : _data) x *= scalar;
  return *this;
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
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& other)
{
  if (_rows != other._rows || _cols != other._cols) throw std::invalid_argument("Matrix sizes are mismatched!");

  for (size_t idx = 0; idx < _data.size(); ++idx)
    _data[idx] += other._data[idx];
  return *this;
}

template <typename T>
bool Matrix<T>::operator==(const Matrix<T>& other) const
{
  return _rows == other._rows && _cols == other._cols && _data == other._data;
}

template <typename T>
bool Matrix<T>::operator!=(const Matrix<T>& other) const
{
  return !(*this == *other);
}

// Helpers
template <typename T>
std::span<T> Matrix<T>::operator[](int i)
{
  return row_at(i);
}

template <typename T>
inline T& Matrix<T>::at(size_t r, size_t c)
{
  if (r >= _rows  || c >= _cols ) throw std::out_of_range("Requested position outside of matrix dimensions.");
  return _data[r * _cols + c];
}

template <typename T>
inline const T& Matrix<T>::at(size_t r, size_t c) const
{
  if (r >= _rows  || c >= _cols ) throw std::out_of_range("Requested position outside of matrix dimensions.");
  return _data[r * _cols + c];
}

template <typename T>
std::span<T> Matrix<T>::row_at(size_t r)
{
  if (r >= _rows) throw std::out_of_range("Requested position outside of matrix dimensions.");
  return std::span(&_data[r * _cols], _cols);
}

template <typename T>
std::span<const T> Matrix<T>::row_at(size_t r) const
{
  if (r >= _rows) throw std::out_of_range("Requested position outside of matrix dimensions.");
  return std::span(&_data[r * _cols], _cols);
}

// Row Operations
template <typename T>
void Matrix<T>::swap_rows(size_t r1, size_t r2)
{
  if (r1 >= rows() || r2 >= rows()) throw std::out_of_range("Requested position outside of matrix dimensions.");

  auto row1_it = data().begin() + r1 * cols();
  auto row1_end = data().begin() + r1 * cols() + cols();

  auto row2_begin = data().begin() + r2 * cols();

  std::swap_ranges(row1_it, row1_end, row2_begin);
}

template <typename T>
void Matrix<T>::scale_row(size_t r, const T& scalar)
{
  if (r >= rows()) throw std::out_of_range("Requested position outside of matrix dimensions.");

  std::for_each(
      data().begin() + r * cols(),
      data().begin() + r * cols() + cols(),
      [scalar](T& x) { x *= scalar; }
  );
}

template <typename T>
void Matrix<T>::add_row(size_t r1, size_t r2, const T& scalar)
{
  if (r1 >= rows() || r2 >= rows()) throw std::out_of_range("Requested position outside of matrix dimensions.");

  auto row1_it = data().begin() + r1 * cols();
  auto row1_end = data().begin() + r1 * cols() + cols();

  auto row2_begin = data().begin() + r2 * cols();

  std::transform(
    row1_it, row1_end,            // first range
    row2_begin, row2_begin,       // second range (target)
    [scalar](const T& a, T& b) { 
      b += scalar * a;            // r2(i) += r1(i) * scalar
      return b;
    });
}

// Linear Algebra Utility
template <typename T>
Matrix<T>::RrefResult Matrix<T>::rref_stats() const
{
  // NOTE: POSSIBLY ADD STATIC_ASSERT TO FORCE FLOATINZG POINT
  Matrix<T> m(*this);
  size_t swaps = 0;
  T scale_prod = T{1};

  size_t c = 0;
  
  for (size_t r = 0; r < m.rows(); ++r)
  {
    if (c >= m.cols()) break;

    // Find a row `r` with non-zero in column[c]
    size_t i = r;
    while (i < m.rows() && m.at(i, c) == T{})
      ++i;

    // Move to next col if no pivot found
    if (i == m.rows())
    {
      ++c;
      --r; // redo row
      continue;
    }

    // Swap pivot row up
    if (i != r) 
    { 
      m.swap_rows(i, r);
      ++swaps;
    }

    const T pivot_val = m.at(r, c);
    if (pivot_val != T{1}) {
      m.scale_row(r, T{1} / pivot_val);
      scale_prod *= pivot_val;
    }

    // Eliminate column in other rows
    for (size_t row_idx = 0; row_idx < m.rows(); ++row_idx)
    {
      if (row_idx == r) continue;
      const T f = m.at(row_idx, c);
      if (f != T{}) m.add_row(r, row_idx, -f);
    }

    // Now move on to next column
    ++c;
  }
  return RrefResult{std::move(m), swaps, scale_prod};
}

template <typename T>
Matrix<T> Matrix<T>::rref() const
{
  return rref_stats().m;
}

template <typename T>
T Matrix<T>::det() const
{
  if (rows() != cols()) throw std::invalid_argument("Finding a determinant requires a square matrix.");
  if (rows() == 1) return data()[0];
  
  // RrefResult represents the collected result of performing an rref operation
  RrefResult res = rref_stats();
  T det = T{1};

  // Compute determinant by calculating product of diagonal of rref matrix
  for (size_t idx = 0; idx < rows(); ++idx)
    det *= res.m.at(idx, idx);

  // Then invert if num of swaps is not divisible by 2
  // (we're flipping for every row swap)
  det = res.swaps % 2 ? -det : det;

  // Then multiply by collected row scalings
  det *= res.scale_prod;

  return det;
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

#endif
