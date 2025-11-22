#pragma once

#ifndef WOJI_RATIONAL_HPP
#define WOJI_RATIONAL_HPP

#include <iostream>
#include <stdexcept>

class Rational{
private:
  int _numerator;
  int _denominator;

  /**
   * @brief Reduces the Rational to its simplest form.
   */
  void reduce()
  {
    int a = (_numerator > _denominator) ? _numerator : _denominator;
    int b = (_numerator > _denominator) ? _denominator : _numerator;

    while (b != 0)
    {
      int r = a % b;
      a = b;
      b = r;
    }
    _numerator /= a;
    _denominator /= a;
  }
public:

  // ==============================================================================
  // Constructors
  // ==============================================================================
  /**
   * @brief Constructs a Rational from numerator and denominator.
   *
   * @param numerator Numerator
   * @param denominator Denominator
   *
   * @throws std::invalid_argument If @p denominator is zero.
   *
   */
  Rational(int numerator, int denominator) : _numerator(numerator), _denominator(denominator) 
  {
    if (denominator == 0)
      throw std::invalid_argument("Denominator cannot be zero!");

    reduce();
  }

  /**
   * @brief Constructs a rational number from a value, with the denominator
   * as '1'.
   *
   * @param value Numerator
   */
  Rational(int value) : _numerator(value), _denominator(1) 
  {
    reduce();
  }

  Rational() : _numerator(0), _denominator(1) { }

  // ==============================================================================
  // Arithmetic
  // ==============================================================================
  Rational operator-() const {
    return Rational(_numerator, _denominator);
  }

  /**
   * @brief Multiplies this rational by another rational and returns the result.
   *
   * @param other The rational to multiply with this rational.
   * @returns A new Rational containing the product of the two rationals.
   */
  Rational operator*(const Rational& other) const;

  /**
   * @brief Multiplies this rational by an integer and returns the result.
   *
   * @param other The integer to multiply with this ratinoal.
   * @returns A new Rational containing the product of this rational and the integer.
   */
  Rational operator*(const int other) const;

  /**
   * @brief Multiplies this rational by another rational in place.
   *
   * @param other The rational to multiply with this rational.
   * @returns Reference to this rational after multiplication.
   */
  Rational& operator*=(const Rational& other);

  /**
   * @brief Multiplies this rational by an integer in place.
   *
   * @param other The integer to multiply with this rational.
   * @returns Reference to this rational after multiplication.
   */
  Rational& operator*=(const int other);

  /**
   * @brief Divides this rational by another rational and returns the result.
   *
   * @param other The rational to divide this rational by.
   * @returns A new Rational containing the quotient of dividing this rational
   * by the other.
   *
   * @throws std::invalid_argument If result would have a denominator of zero.
   */
  Rational operator/(const Rational& other) const;

  /**
   * @brief Divides this rational by an integer and returns the result.
   * 
   * @param other The integer to divide this rational by.
   * @returns A new Rational containing the quotient of dividing this rational
   * by the integer.
   *
   * @throws std::invalid_argument If @p other is zero.
   */
  Rational operator/(const int other) const;

  /**
   * @brief Divides this rational by another rational in place.
   *
   * @param other The rational to divide this rational by.
   * @returns Reference to this rational after division.
   */
  Rational& operator/=(const Rational& other);

  /**
   * @brief Divides this rational by an integer in place.
   *
   * @param other The integer to divide this rational by.
   * @returns Reference to this rational after division.
   *
   * @throws std::invalid_argument If @p other is zero.
   */
  Rational& operator/=(const int other);

  /**
   * @brief Adds this rational to another rational and returns the result.
   *
   * @param other The rational to add to this rational.
   * @return A new Rational representing the sum of the two rationals.
   */ 
  Rational operator+(const Rational& other) const;

  /**
   * @brief Adds an integer value to this rational and returns the result.
   *
   * @param other The integer to add to this rational.
   * @return A new rational representing the sum of the two rationals.
   */
  Rational operator+(const int other) const;

  /**
   * @brief Adds another rational to this rational in place.
   *
   * @param other The rational to add to this rational.
   * @return Reference to this rational after addition.
   */
  Rational& operator+=(const Rational& other);

  /**
   * @brief Adds an integer to this rational in place.
   *
   * @param other The integer value to add to this rational.
   * @return Reference to this rational after addition.
   */
  Rational& operator+=(const int other);

  // ==============================================================================
  // Operator Overloads
  // ==============================================================================

  /**
   * @brief Checks whether this rational is equal to another rational.
   *
   * @param other The rational to compare with this rational.
   * @returns True if both rationals are equal. False otherwise.
   */
  bool operator==(const Rational& other) const;

  /**
   * @brief Checks whether this rational is not equal to another rational.
   *
   * @param other The rational to compare with this rational.
   * @returns True if both rationals are not equal. False otherwise.
   */
  bool operator!=(const Rational& other) const;

  // ==============================================================================
  // Accessors
  // ==============================================================================
  int& numerator();
  const int numerator() const;
  int& denominator();
  const int denominator() const;

  // ==============================================================================
  // Printing Utility
  // ==============================================================================
  
  /** Prints the rational to the standard output stream. */
  void print() const;

};

// ==============================================================================
// Arithmetic
// ==============================================================================
inline Rational Rational::operator*(const Rational& other) const
{
  return Rational(_numerator * other._numerator, _denominator * other._denominator);
}

inline Rational Rational::operator*(const int other) const
{
  return Rational(_numerator * other, _denominator);
}

inline Rational& Rational::operator*=(const Rational& other)
{
  _numerator *= other._numerator;
  _denominator *= other._denominator;
  reduce();
  return *this;
}

inline Rational& Rational::operator*=(const int other)
{
  _numerator *= other;
  reduce();
  return *this;
}

inline Rational Rational::operator/(const Rational& other) const
{
  if (other._numerator == 0)
    throw std::invalid_argument("Resulting rational would have a denominator of zero!");

  return Rational(_numerator * other._denominator, _denominator * other._numerator);
}

inline Rational Rational::operator/(const int other) const
{
  if (other == 0)
    throw std::invalid_argument("Cannot divide by zero!");

  return Rational(_numerator, _denominator * other);
}

inline Rational& Rational::operator/=(const Rational& other)
{
  if (other._numerator == 0)
    throw std::invalid_argument("Resulting rational would have a denominator of zero!");

  _numerator *= other._denominator;
  _denominator *= other._numerator;

  reduce();
  return *this;
}

inline Rational& Rational::operator/=(const int other)
{
  if (other == 0)
    throw std::invalid_argument("Cannot divide by zero!");

  _denominator *= other;
  reduce();
  return *this;
}

inline Rational Rational::operator+(const Rational& other) const
{
  return Rational(_numerator * other._denominator + other._numerator * _denominator, _denominator * other._denominator);
}

inline Rational Rational::operator+(const int other) const
{
  return Rational(_numerator + other * _denominator, _denominator);
}

inline Rational& Rational::operator+=(const Rational& other)
{
  _numerator = _numerator * other._denominator + other._numerator * _denominator;
  _denominator = _denominator * other._denominator;
  reduce();
  return *this;
}

inline Rational& Rational::operator+=(const int other)
{
  _numerator += other * _denominator;
  reduce();
  return *this;
}

// ==============================================================================
// Operator Overloads
// ==============================================================================
inline bool Rational::operator==(const Rational& other) const
{
  return _numerator == other._numerator && _denominator == other._denominator;
}

inline bool Rational::operator!=(const Rational& other) const
{
  return !(*this == other);
}

// ==============================================================================
// Accessors
// ==============================================================================
inline int& Rational::numerator()
{
  return _numerator;
}

inline const int Rational::numerator() const
{
  return _numerator;
}


inline int& Rational::denominator()
{
  return _denominator;
}

inline const int Rational::denominator() const
{
  return _denominator;
}

// ==============================================================================
// Printing Utility
// ==============================================================================
inline void Rational::print() const
{
  std::cout << numerator() << " / " << denominator() << std::endl;
}

inline std::ostream& operator<<(std::ostream& os, const Rational& r)
{
  if (r.denominator() == 1)
    return os << r.numerator();
  return os << r.numerator() << "/" << r.denominator();
}
#endif
