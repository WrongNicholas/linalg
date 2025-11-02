#include <gtest/gtest.h>
#include <lin_alg/Matrix.hpp>
#include <stdexcept>

TEST(MatrixTest, ConstructsWithDimensions_DefaultInitializesElements)
{
  Matrix<std::string> m(1, 2);

  EXPECT_EQ(m.rows(), 1u);
  EXPECT_EQ(m.cols(), 2u);

  EXPECT_EQ(m(0,0), ""); 
  EXPECT_EQ(m(0,1), ""); 
}

TEST(MatrixTest, ConstructsWithDimensions_ZeroRowsOrCols_Throws)
{
  EXPECT_THROW(Matrix<std::string> m(0, 2), std::invalid_argument);
  EXPECT_THROW(Matrix<std::string> m(2, 0), std::invalid_argument);
}

TEST(MatrixTest, ConstructsFromFlatInitializer_RowMajorOrder)
{
  Matrix<std::string> m(2, 2, {"0", "1", "2", "3"});
  // 0 1
  // 2 3

  // row 0
  EXPECT_EQ(m(0,0), "0"); 
  EXPECT_EQ(m(0,1), "1"); 

  // row 1
  EXPECT_EQ(m(1,0), "2"); 
  EXPECT_EQ(m(1,1), "3"); 

  EXPECT_EQ(m.rows(), 2u);
  EXPECT_EQ(m.cols(), 2u);
}

TEST(MatrixTest, ConstructsFromFlatInitializer_WrongSize_Throws)
{
  EXPECT_THROW(Matrix<std::string> m(2, 2, {"0", "1", "2"}), std::invalid_argument);
  EXPECT_THROW(Matrix<std::string> m(2, 2, {"0", "1", "2", "3", "4"}), std::invalid_argument);
}

TEST(MatrixTest, ConstructsFromFlatInitializer_WithZeroDimension_Throws)
{
  EXPECT_THROW(Matrix<std::string> m(0, 2, {}), std::invalid_argument);
  EXPECT_THROW(Matrix<std::string> m(2, 0, {}), std::invalid_argument);
}

TEST(MatrixTest, ConstructsFromColumnsInitializer_InnerAsRows)
{
  Matrix<std::string> m({
      {"0", "1", "2"},
      {"3", "4", "5"}
  });
  // Looks like:
  // 0 1 2
  // 3 4 5

  EXPECT_EQ(m.rows(), 2u);
  EXPECT_EQ(m.cols(), 3u);

  // column 0
  EXPECT_EQ(m(0,0), "0"); 
  EXPECT_EQ(m(1,0), "3"); 
  
  // column 1
  EXPECT_EQ(m(0,1), "1"); 
  EXPECT_EQ(m(1,1), "4"); 

  // column 2
  EXPECT_EQ(m(0,2), "2");
  EXPECT_EQ(m(1,2), "5");
}

TEST(MatrixTest, ConstructsFromRowsInitializer_MisalignedRows_Throws)
{
  EXPECT_THROW(Matrix<std::string> m({{"0", "1"}, {"2"}}), std::invalid_argument);
}

TEST(MatrixTest, ConstructsFromRowsInitializer_EmptyOuterList_Throws)
{
  EXPECT_THROW(Matrix<std::string> m({}), std::invalid_argument);
}

TEST(MatrixTest, MultiplicationOverload)
{
  // 2 x 3 matrix
  Matrix<int> A = { 
    {0, 2, 4},
    {1, 3, 5}
  };
  // 0 2 4
  // 1 3 5
  
  // 3 x 2 matrix
  Matrix<int> B = {
    {6, 9},
    {7, 10},
    {8, 11}
  };
  // 6 9
  // 7 10
  // 8 11

  Matrix<int> expected = {
    {46, 64},
    {67, 94}
  };
  // 46 64
  // 67 94
  
  Matrix<int> actual = A*B;

  ASSERT_TRUE(actual == expected);
}

TEST(MatrixTest, MultiplicationOverload_MismatchedSizes_Throws)
{
  // 2 x 3 matrix
  Matrix<int> A = { 
    {0,2,4},
    {1,3,5}
  };
  // 0 2 4
  // 1 3 5
  
  // 3 x 2 matrix
  Matrix<int> B = {
    {6, 9},
    {7,10},
  };
  // 6 9
  // 7 10

  ASSERT_THROW(Matrix<int> actual = A*B, std::invalid_argument);

}

TEST(MatrixTest, AdditionOverload)
{
  // 2 x 3 matrix
  Matrix<int> A = { 
    {0,1}, 
    {2,3},
    {4,5}
  };
  // 0 1
  // 2 3
  // 4 5
  
  // 2 x 3 matrix
  Matrix<int> B = { 
    {6, 7}, 
    {8, 9},
    {10,11}
  };
  // 6 7
  // 8 9
  // 10 11

  Matrix<int> expected = {
    {6, 8}, 
    {10,12},
    {14,16}
  };
  // 6 8
  // 10 12
  // 14 16

  Matrix<int> actual = A+B;

  ASSERT_TRUE(actual == expected);
}

TEST(MatrixTest, AdditionOverload_MismatchedSizes_Throws)
{
  // 2 x 3 matrix
  Matrix<int> A = { 
    {0,1}, 
    {2,3},
    {4,5}
  };
  // 0 1
  // 2 3
  // 4 5
  
  // 2 x 3 matrix
  Matrix<int> B = { 
    {6, 7}, 
    {8, 9}
  };
  // 6 7
  // 8 9

  ASSERT_THROW(Matrix<int> actual = A+B, std::invalid_argument);
}

TEST(MatrixTest, MultiplicationOverload_Scalar)
{
  // 2 x 3 matrix
  Matrix<int> A = { 
    {0,1}, 
    {2,3},
    {4,5}
  };
  // 0 1
  // 2 3
  // 4 5

  int scalar = 10;

  Matrix<int> expected = {
    {0,10}, 
    {20,30},
    {40,50}
  };

  Matrix<int> actual = A*scalar;

  ASSERT_TRUE(actual == expected);
}

TEST(MatrixTest, RowSwap)
{
  // 2 x 3 matrix
  Matrix<int> A = { 
    {0,1}, 
    {2,3},
    {4,5}
  };
  // 0 1
  // 2 3
  // 4 5

  A.swap_rows(0, 1);
  // 2 3
  // 0 1
  // 4 5

  A.swap_rows(1, 2);
  // 2 3
  // 4 5
  // 0 1

  A.swap_rows(0, 1);
  // 4 5
  // 2 3
  // 0 1

  Matrix<int> expected = { 
    {4,5},
    {2,3},
    {0,1} 
  };

  ASSERT_TRUE(expected == A);
}

TEST(MatrixTest, RowSwap_OutsideBounds_Throws)
{
  // 2 x 3 matrix
  Matrix<int> A = { 
    {0,1}, 
    {2,3},
    {4,5}
  };
  // 0 1
  // 2 3
  // 4 5

  ASSERT_THROW(A.swap_rows(0, 3), std::out_of_range);
}

TEST(MatrixTest, RowScale)
{
  // 2 x 3 matrix
  Matrix<int> A = { 
    {0,1}, 
    {2,3},
    {4,5}
  };
  // 0 1
  // 2 3
  // 4 5

  A.scale_row(0, 10);
  // 0 10
  // 2 3
  // 4 5

  A.scale_row(1, 5);
  // 0 10
  // 10 15
  // 4 5

  A.scale_row(2, 2);
  // 0 10
  // 10 15
  // 8 10

  Matrix<int> expected = { 
    {0,10}, 
    {10,15},
    {8,10}
  };

  ASSERT_TRUE(A == expected);
}

TEST(MatrixTest, RowAddition)
{
  // 2 x 3 matrix
  Matrix<int> A = { 
    {0,1}, 
    {2,3},
    {4,5}
  };
  // 0 1
  // 2 3
  // 4 5

  // Add row 0, multiplied by a scalar of 10, to row 1
  A.add_row(0, 1, 10);
  // 0 1
  // 2 13
  // 4 5

  // Add row 2, multiplied by a scalar of 2, to row 0
  A.add_row(2, 0, 2);
  // 8 11
  // 2 13
  // 4 5

  // Add row 1, multiplied by a scalar of 0, to row 0
  A.add_row(1, 0, 0);
  // 8 11
  // 2 13
  // 4 5
  Matrix<int> expected = { 
    {8, 11}, 
    {2, 13},
    {4, 5}
  };

  ASSERT_TRUE(A == expected);
}

TEST(MatrixTest, FromColumnFactory)
{
  // 3 x 2 matrix
  auto m = Matrix<int>::from_columns({
    {0, 1},
    {2, 3},
    {4, 5}
  });
  // 0 2 4
  // 1 3 5
  
  auto expected = Matrix<int>({
    {0, 2, 4},
    {1, 3, 5}
  });

  ASSERT_TRUE(m == expected);
}

TEST(MatrixTest, rref)
{
  //! [rref_example]
  // Construct a 3x4 augmented matrix representing a linear system:
  // x - 2y +  z = 0
  //     2y - 8z = 8
  // 5x     - 5z = 10
  Matrix<double> m({
    {1,-2, 1, 0},
    {0, 2,-8, 8},
    {5, 0,-5,10},
  });

  // Compute the Reduces Row Echelon Form (RREF)
  auto rref = m.rref();

  // The resulting matrix corresponds to:
  // x = 1
  // y = 0
  // z = -1
  Matrix<double> expected({
    {1, 0, 0, 1},
    {0, 1, 0, 0},
    {0, 0, 1,-1},
  });
  //! [rref_example]

  ASSERT_TRUE(rref == expected);
}
