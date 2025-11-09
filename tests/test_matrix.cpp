#include <gtest/gtest.h>
#include <lin_alg/Matrix.hpp>
#include <lin_alg/Rational.hpp>
#include <stdexcept>

// ==============================================================================
// Constructor Tests
// ==============================================================================

TEST(MatrixTest, ConstructsWithDimensions_DefaultInitializesElements)
{
  Matrix<std::string> m(1, 2);

  EXPECT_EQ(m.rows(), 1u);
  EXPECT_EQ(m.cols(), 2u);

  EXPECT_EQ(m[0][0], ""); 
  EXPECT_EQ(m[0][1], ""); 
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
  EXPECT_EQ(m.at(0,0), "0"); 
  EXPECT_EQ(m.at(0,1), "1"); 

  // row 1
  EXPECT_EQ(m.at(1,0), "2"); 
  EXPECT_EQ(m.at(1,1), "3"); 

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
  Matrix<std::string> m({ {"0", "1", "2"}, {"3", "4", "5"} });
  // 0 1 2
  // 3 4 5

  EXPECT_EQ(m.rows(), 2u);
  EXPECT_EQ(m.cols(), 3u);

  EXPECT_EQ(m.at(0,0), "0"); 
  EXPECT_EQ(m.at(1,0), "3"); 
  EXPECT_EQ(m.at(0,1), "1"); 
  EXPECT_EQ(m.at(1,1), "4"); 
  EXPECT_EQ(m.at(0,2), "2");
  EXPECT_EQ(m.at(1,2), "5");
}

TEST(MatrixTest, ConstructsFromRowsInitializer_MisalignedRows_Throws)
{
  EXPECT_THROW(Matrix<std::string> m({{"0", "1"}, {"2"}}), std::invalid_argument);
}

TEST(MatrixTest, ConstructsFromRowsInitializer_EmptyOuterList_Throws)
{
  EXPECT_THROW(Matrix<std::string> m({}), std::invalid_argument);
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

// ==============================================================================
// Copy, Access, Span
// ==============================================================================

TEST(MatrixTest, CopyConstructor_CreatesDeepCopy)
{
  Matrix<int> original = {{1,2}, {3,4}};
  Matrix<int> copy = original;

  copy.at(0, 0) = 1234;
  EXPECT_EQ(original.at(0,0), 1);
  EXPECT_EQ(copy.at(0,0), 1234);
}

TEST(MatrixTest, ElementAccess_OutOfRange_Throws)
{
  Matrix<int> m(2,2);
  EXPECT_THROW(m.at(2,0), std::out_of_range);
  EXPECT_THROW(m.at(0,2), std::out_of_range);
}

TEST(MatrixTest, RowAt_ReturnsCorrectSpan)
{
  Matrix<int> m({{1,2,3}, {4,5,6}});
  auto row0 = m.row_at(0);
  EXPECT_EQ(row0.size(), 3u);
  row0[1] = 1234;
  EXPECT_EQ(m.at(0,1), 1234);
}

// ============================================================================
//  Arithmetic
// ============================================================================

TEST(MatrixTest, AdditionOverload)
{
  Matrix<int> A = { {0,1}, {2,3}, {4,5} };
  Matrix<int> B = { {6, 7}, {8, 9}, {10,11} };

  Matrix<int> expected = { {6, 8}, {10,12}, {14,16} };
  Matrix<int> actual = A+B;

  ASSERT_TRUE(actual == expected);
}

TEST(MatrixTest, AdditionOverload_MismatchedSizes_Throws)
{
  Matrix<int> A = { {0,1}, {2,3}, {4,5} };
  Matrix<int> B = { {6, 7}, {8, 9} };
  ASSERT_THROW(Matrix<int> actual = A+B, std::invalid_argument);
}

TEST(MatrixTest, AdditionOperator_ChainAddition)
{
  Matrix<int> A = {{1,2},{3,4}};
  Matrix<int> B = {{5,6},{7,8}};
  Matrix<int> C = {{1,1},{1,1}};
  Matrix<int> expected = {{7,9}, {11,13}};
  Matrix<int> actual = A+B+C;
  ASSERT_TRUE(actual == expected);
}

TEST(MatrixTest, MultiplicationOverload)
{
  Matrix<int> A = { {0, 2, 4}, {1, 3, 5} };
  
  Matrix<int> B = { {6, 9}, {7, 10}, {8, 11} };

  Matrix<int> expected = { {46, 64}, {67, 94} };
  Matrix<int> actual = A*B;

  ASSERT_TRUE(actual == expected);
}

TEST(MatrixTest, MultiplicationOverload_MismatchedSizes_Throws)
{
  Matrix<int> A = { {0,2,4}, {1,3,5} };
  Matrix<int> B = { {6, 9}, {7,10}, };
  ASSERT_THROW(Matrix<int> actual = A*B, std::invalid_argument);
}

TEST(MatrixTest, MatrixMultiplication_ByIdentity)
{
  Matrix<int> A = {{1,2},{3,4}};
  Matrix<int> I = {{1,0},{0,1}};
  ASSERT_TRUE((A*I) == A);
}

TEST(MatrixTest, ScalarMultiplication_ByZero)
{
  Matrix<int> A = {{1,2},{3,4}};
  Matrix<int> expected = {{0,0},{0,0}};
  ASSERT_TRUE((A*0) == expected);
}

TEST(MatrixTest, MultiplicationOverload_Scalar)
{
  Matrix<int> A = { {0,1}, {2,3}, {4,5} };
  int scalar = 10;

  Matrix<int> expected = { {0,10}, {20,30}, {40,50} };
  Matrix<int> actual = A*scalar;

  ASSERT_TRUE(actual == expected);
}

// ============================================================================
//  Row Operations
// ============================================================================

TEST(MatrixTest, RowSwap)
{
  Matrix<int> A = { {0,1}, {2,3}, {4,5} };
  A.swap_rows(0, 1);
  A.swap_rows(1, 2);
  A.swap_rows(0, 1);
  Matrix<int> expected = { {4,5}, {2,3}, {0,1} };
  ASSERT_TRUE(expected == A);
}

TEST(MatrixTest, RowSwap_OutsideBounds_Throws)
{
  Matrix<int> A = { {0,1}, {2,3}, {4,5} };
  ASSERT_THROW(A.swap_rows(0, 3), std::out_of_range);
}

TEST(MatrixTest, ScaleRow)
{
  Matrix<int> A = { {0,1}, {2,3}, {4,5} };
  A.scale_row(0, 10);
  A.scale_row(1, 5);
  A.scale_row(2, 2);
  Matrix<int> expected = { {0,10}, {10,15}, {8,10} };
  ASSERT_TRUE(A == expected);
}

TEST(MatrixTest, ScaleRow_ByZero)
{
  Matrix<int> A = {{1,2},{3,4}};
  A.scale_row(0,0);
  Matrix<int> expected = {{0,0},{3,4}};
  ASSERT_TRUE(A == expected);
}

TEST(MatrixTest, AddRow)
{
  Matrix<int> A = { {0,1}, {2,3}, {4,5} };
  A.add_row(0, 1, 10);
  A.add_row(2, 0, 2);
  A.add_row(1, 0, 0);
  Matrix<int> expected = { {8, 11}, {2, 13}, {4, 5} };
  ASSERT_TRUE(A == expected);
}

TEST(MatrixTest, AddRow_OutsideBounds_Throws)
{
  Matrix<int> A = {{1,2},{3,4}};
  EXPECT_THROW(A.add_row(2,0,1), std::out_of_range);
  EXPECT_THROW(A.add_row(0,2,1), std::out_of_range);
}

// ============================================================================
//  Linear Algebra
// ============================================================================

TEST(MatrixTest, Rref)
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

TEST(MatrixTest, Rref_IdentityMatrixUnchanged)
{
  Matrix<double> I({
    {1,0,0},
    {0,1,0},
    {0,0,1}
  });
  ASSERT_TRUE(I.rref() == I);
}

TEST(MatrixTest, Rref_UpperTriangle)
{
  Matrix<double> M({
    {2,1},
    {0,3}
  });
  Matrix<double> expected({
    {1, 0},
    {0, 1}
  });
  ASSERT_TRUE(M.rref() == expected);
}

TEST(MatrixTest, Determinant_TwoByTwo)
{
  Matrix<double> m({
    {1, 2},
    {3, 4}
  });

  auto actual = m.det();
  double expected = -2;

  ASSERT_EQ(actual, expected);
}

TEST(MatrixTest, Determinant_FourByFour)
{
  Matrix<double> m({
      {1,-2, 1, 0},
      {0, 2,-8, 8},
      {5, 0,-5,10},
      {9, -5,-5,6},
    });

  auto actual = m.det();
  double expected = -480;

  ASSERT_EQ(actual, expected);
}

TEST(MatrixTest, Determinant_Misaligned_Throws)
{
  
  Matrix<double> m({
    {1,-2, 1, 0},
    {0, 2,-8, 8},
    {5, 0,-5,10}
  });

  ASSERT_THROW(m.det(), std::invalid_argument);
}

TEST(MatrixTest, Determinant_OneByOne)
{
  Matrix<double> M({{7}});
  EXPECT_EQ(M.det(), 7);
}

TEST(MatrixTest, DeterminantZero)
{
  Matrix<double> M({
    {1,2},
    {2,4}
  });

  EXPECT_EQ(M.det(), 0);
}

TEST(MatrixTest, LinearlyIndependent)
{
  Matrix<double> m({
    {1,-2, 1},
    {0, 2,-8},
    {5, 0,-5}
  });

  ASSERT_TRUE(m.linearly_independent());
}

TEST(MatrixTest, LinearlyDependent)
{

  Matrix<double> M({
    {1,2},
    {2,4}
  });

  ASSERT_FALSE(M.linearly_independent());
}

TEST(MatrixTest, Solution)
{
  Matrix<double> m({
    {1,-2, 1},
    {0, 2,-8},
    {5, 0,-5},
  });

  std::vector<const double> b({0,8,10});

  std::vector<double> actual = *m.solution(b);
  std::vector<double> expected({1, 0, -1});

  ASSERT_TRUE(actual == expected);
}

TEST(MatrixTest, Solution_MismatchedSizes_Throws)
{
  Matrix<double> m({
    {1,-2, 1},
    {0, 2,-8},
    {5, 0,-5},
  });

  std::vector<const double> b({0,8});

  EXPECT_THROW(*m.solution(b), std::invalid_argument);
}

TEST(MatrixTest, Solution_BIG)
{
  Matrix<double> m({
    {2, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 3, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 3, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 3, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 3, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 3, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 3, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 3, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 3, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 2},
  });

  std::vector<double> b({0,-2,3,-4,5,-6,7,-8,9,-11});

  std::vector<double> actual = *m.solution(b);
  std::vector<double> expected({1,-2,3,-4,5,-6,7,-8,9,-10});

  // REALLY NEED TO WRITE RATIONAL CLASS
  for (size_t idx = 0; idx < 10; ++idx)
    ASSERT_NEAR(actual[idx], expected[idx], 1e-12);
}

// ============================================================================
// Utility
// ============================================================================
TEST(MatrixTest, EqualityOperator_FalseForDifferentSizes)
{
  Matrix<int> A = {{1,2},{3,4}};
  Matrix<int> B = {{5,6,7}};

  EXPECT_FALSE(A == B);
}

// ============================================================================
// Rational Testing
// ============================================================================
TEST(MatrixTest, Rational)
{
  Matrix<Rational> A{{Rational(1,1), Rational(1,2)}, {Rational(2,1), Rational(2,2)}};

  EXPECT_TRUE(A.at(0,0) == Rational(1,1));
}
