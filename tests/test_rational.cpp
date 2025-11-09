#include <gtest/gtest.h>
#include <lin_alg/Rational.hpp>

TEST(RationalTest, Constructs)
{
  Rational r(1,2);

  ASSERT_EQ(r.numerator(), 1);
  ASSERT_EQ(r.denominator(), 2);
}

TEST(RationalTest, Constructs_ZeroInDenominator_Throws)
{
  ASSERT_THROW(Rational r(1,0), std::invalid_argument);
}

TEST(RationalTest, Constructs_FromInteger)
{
  Rational r(10);

  ASSERT_EQ(r.numerator(), 10);
  ASSERT_EQ(r.denominator(), 1);
}

TEST(RationalTest, MultiplicationOverload)
{
  Rational r1(1, 5);
  Rational r2(1, 2);

  auto prod1 = r1 * r2;
  ASSERT_EQ(prod1.numerator(), 1);
  ASSERT_EQ(prod1.denominator(), 10);

  Rational r3(1, 5);

  auto prod2 = r3 * 2;
  ASSERT_EQ(prod2.numerator(), 2);
  ASSERT_EQ(prod2.denominator(), 5);

  Rational r4(5, 2);
  Rational r5(3, 7);

  r4 *= r5;
  ASSERT_EQ(r4.numerator(), 15);
  ASSERT_EQ(r4.denominator(), 14);

  Rational r6(7,3);
  r6 *= 2;

  ASSERT_EQ(r6.numerator(), 14);
  ASSERT_EQ(r6.denominator(), 3);
}

TEST(RationalTest, DivisionOverload)
{
  Rational r1(3,2);
  Rational r2(2,7);

  auto quot1 = r1/r2;
  ASSERT_EQ(quot1.numerator(), 21);
  ASSERT_EQ(quot1.denominator(), 4);

  Rational r3(3,2);
  
  auto quot2 = r3/4;
  ASSERT_EQ(quot2.numerator(), 3);
  ASSERT_EQ(quot2.denominator(), 8);

  r1 /= r2;
  ASSERT_EQ(r1.numerator(), 21);
  ASSERT_EQ(r1.denominator(), 4);

  r1 /= 2;
  ASSERT_EQ(r1.numerator(), 21);
  ASSERT_EQ(r1.denominator(), 8);
}

TEST(RationalTest, AdditionOverload)
{
  Rational r1(5,7);
  Rational r2(2,3);

  auto sum1 = r1 + r2;
  ASSERT_EQ(sum1.numerator(), 29);
  ASSERT_EQ(sum1.denominator(), 21);

  auto sum2 = r1 + 2;
  ASSERT_EQ(sum2.numerator(), 19);
  ASSERT_EQ(sum2.denominator(), 7);

  r1 += r2;
  ASSERT_EQ(r1.numerator(), 29);
  ASSERT_EQ(r1.denominator(), 21);

  r2 += 2;
  ASSERT_EQ(r2.numerator(), 8);
  ASSERT_EQ(r2.denominator(), 3);
}

TEST(RationalTest, EqualityOverload)
{
  Rational r1(1,2);
  Rational r2(5,8);

  ASSERT_FALSE(r1 == r2);
  ASSERT_TRUE(r1 != r2);

  Rational r3(1,2);

  ASSERT_TRUE(r1 == r3);
  ASSERT_FALSE(r2 == r3);
}
