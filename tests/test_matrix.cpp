#include <gtest/gtest.h>
#include <lin_alg/Matrix.hpp>

// Initializer List Construction
TEST(MatrixTest, InitializesFromInitializerList)
{
  // Constructs a matrix from an initializer list
  Matrix<int> m(2, 2, {0, 1, 2, 3});
  EXPECT_EQ(m(0,0), 0); 
  EXPECT_EQ(m(1,0), 1); 
  EXPECT_EQ(m(0,1), 2); 
  EXPECT_EQ(m(1,1), 3); 
}
