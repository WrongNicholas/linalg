#include <gtest/gtest.h>
#include <lin_alg/Matrix.hpp>

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
