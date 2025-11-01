#include "Matrix.hpp"

int main() {

  /* Defines a matrix:
   * 0 2 4
   * 1 3 5 */
  std::cout << "Define and print matrix from initializer_list {0, 1, 2, 3, 4, 5}:" << std::endl;
  Matrix<int> matrix = Matrix<int>(2, 3, {0, 1, 2, 3, 4, 5});
  matrix.print();
  
  std::cout << "Matrix at column 0:" << std::endl;
  matrix.print_col(0);
}
