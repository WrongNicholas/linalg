#include <iostream>
#include <vector>
#include <optional>
#include <lin_alg/Matrix.hpp>
#include <lin_alg/Rational.hpp>

int main()
{
  // Define three vectors:
  std::vector<Rational> v1 = { Rational(1), Rational(0), Rational(5)};
  std::vector<Rational> v2 = { Rational(-2), Rational(2), Rational(0)};
  std::vector<Rational> v3 = { Rational(1),Rational(-8),Rational(-5)};

  // Construct matrix A with the vectors as columns
  Matrix<Rational> A = Matrix<Rational>::from_columns({v1,v2,v3});
  A.print();

  // // Check linear independence
  bool is_lin_indep = A.linearly_independent();
  std::cout << "\nA is" 
    << (is_lin_indep ? " " : " NOT ") 
    << "linearly independent.\n\n";

  // // Define three vectors:
  // std::vector<double> v1 = { 1, 0, 5};
  // std::vector<double> v2 = {-2, 2, 0};
  // std::vector<double> v3 = { 1,-8,-5};
  //
  // std::cout << "v1 = {1 ,0 ,5}, v2 = {-2, 2, 0}, v3={1, -8, -5}\n\n";
  //
  // // Construct matrix A with the vectors as columns
  // Matrix<double> A = Matrix<double>::from_columns({v1, v2, v3});
  // std::cout << "A = \n";
  // A.print();
  //
  // // Check linear independence
  // bool is_lin_indep = A.linearly_independent();
  // std::cout << "\nA is" 
  //   << (is_lin_indep ? " " : " NOT ") 
  //   << "linearly independent.\n\n";
  //
  // // Compute determinant
  // const double determinant = A.det();
  // std::cout << "det(A) = " << determinant << "\n\n";
  //
  // // Define vector b
  // std::vector<double> b = {0, 8, 10};
  // std::cout << "b = {0, 8, 10}\n\n";
  //
  // // Solve Ax = b
  // std::optional<std::vector<double>> solution = A.solution(b);
  // std::cout << "Ax=b; x = ";
  //
  // if (solution) {
  //       for (double x : *solution) {
  //           std::cout << x << " ";
  //       }
  // } else std::cout << "NO SOLUTION";
  // std::cout << "\n\n";
  //
  // // Define matrix B
  // Matrix<double> B({
  //   {1, 2},
  //   {3, 4},
  //   {5, 6}
  // });
  // std::cout << "B = \n";
  // B.print();
  // std::cout << "\n";
  //
  // // Multiply A * B = C
  // const auto C = A * B;
  // std::cout << "A * B = C = \n";
  // C.print();
  //
  // // Define a rational number 1 / 2
  // Rational r(10,2);
  // r.print();

  return 0;
}
