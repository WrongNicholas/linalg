#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <span>
#include <iostream>

template <typename T>
class Matrix {
private:
  const int m;
  const int n;
  std::vector<T> data;
public:
  Matrix(const int m, const int n);

  std::span<T> row_at();


  void print();
};

template <typename T>
Matrix<T>::Matrix(int m, int n) : m(m), n(n), data(m * n) { }

template <typename T>
void Matrix<T>::print()
{
  for (size_t i = 0; i < data.size(); ++i) {
    std::cout << data[i] << ", ";
    std::cout << std::endl;
  }
}

#endif
