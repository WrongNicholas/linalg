#include "Matrix.hpp"

int main() {

  Matrix<int> matrix = Matrix<int>(2, 3, {0, 1, 2, 3, 4, 5});
  matrix.print();

  std::vector<int> vec = matrix.row_at(1);
  for (size_t i = 0; i < vec.size(); ++i)
  {
    std::cout << vec[i] << std::endl;
  }
}
