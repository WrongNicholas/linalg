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

TEST(MatrixTest, ConstructsFromFlatInitializer_ColumnMajorOrder)
{
  Matrix<std::string> m(2, 2, {"0", "1", "2", "3"});

  // column 0
  EXPECT_EQ(m(0,0), "0"); 
  EXPECT_EQ(m(1,0), "1"); 

  // column 1
  EXPECT_EQ(m(0,1), "2"); 
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

TEST(MatrixTest, ConstructsFromColumnsInitializer_InnerAsColumns)
{
  Matrix<std::string> m({
      {"0", "1"}, 
      {"2", "3"}, 
      {"4", "5"}
  });
  // Looks like:
  // 0 2 4
  // 1 3 5

  EXPECT_EQ(m.rows(), 2u);
  EXPECT_EQ(m.cols(), 3u);

  // column 0
  EXPECT_EQ(m(0,0), "0"); 
  EXPECT_EQ(m(1,0), "1"); 
  
  // column 1
  EXPECT_EQ(m(0,1), "2"); 
  EXPECT_EQ(m(1,1), "3"); 

  // column 2
  EXPECT_EQ(m(0,2), "4");
  EXPECT_EQ(m(1,2), "5");
}

TEST(MatrixTest, ConstructsFromColumnsInitializer_MisalignedColumns_Throws)
{
  EXPECT_THROW(Matrix<std::string> m({{"0", "1"}, {"2"}}), std::invalid_argument);
}

TEST(MatrixTest, ConstructsFromColumnsInitializer_EmptyOuterList_Throws)
{
  EXPECT_THROW(Matrix<std::string> m({}), std::invalid_argument);
}

TEST(MatrixTest, MultiplicationOverload)
{
  // 2 x 3 matrix
  Matrix<int> A = { 
    {0,1}, 
    {2,3},
    {4,5}
  };
  // 0 2 4
  // 1 3 5
  
  // 3 x 2 matrix
  Matrix<int> B = {
    {6, 7, 8},
    {9,10,11},
  };
  // 6 9
  // 7 10
  // 8 11

  Matrix<int> expected = {
    {46, 67},
    {64, 94}
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
    {0,1}, 
    {2,3},
    {4,5}
  };
  // 0 2 4
  // 1 3 5
  
  // 3 x 2 matrix
  Matrix<int> B = {
    {6, 7},
    {9,10},
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
  // 0 2 4
  // 1 3 5
  
  // 2 x 3 matrix
  Matrix<int> B = { 
    {6, 7}, 
    {8, 9},
    {10,11}
  };
  // 6 8 10
  // 7 9 11

  Matrix<int> expected = {
    {6, 8}, 
    {10,12},
    {14,16}
  };
  // 6 10 14
  // 8 12 16

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
  // 0 2 4
  // 1 3 5
  
  // 2 x 3 matrix
  Matrix<int> B = { 
    {6, 7}, 
    {8, 9}
  };
  // 6 8
  // 7 9

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
  // 0 2 4
  // 1 3 5

  int scalar = 10;

  Matrix<int> expected = {
    {0,10}, 
    {20,30},
    {40,50}
  };

  Matrix<int> actual = A*scalar;

  actual.print_row_major();
  ASSERT_TRUE(actual == expected);
}
