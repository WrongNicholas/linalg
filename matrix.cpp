#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>

template <typename T>
class matrix {
private:
  std::vector<std::vector<T>> core;
public:
  matrix(const int m, const int n);
};

template <typename T>
matrix<T>::matrix(const int m, const int n)
{
  core = std::vector(m, std::vector<T>(n));
}

#endif
