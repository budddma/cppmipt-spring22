#pragma once
#include <stdexcept>

class RationalDivisionByZero : public std::runtime_error {
 public:
  RationalDivisionByZero() : std::runtime_error("RationalDivisionByZero") {
  }
};

int32_t Gcd(int32_t lhs, int32_t rhs);

class Rational {
 private:
  int32_t numerator_;
  int32_t denominator_;

  void Normalize();

 public:
  Rational();

  Rational(const int32_t& num);  // NOLINT

  Rational(const int32_t& num, const int32_t& denom);

  int32_t GetNumerator() const;

  int32_t GetDenominator() const;

  void SetNumerator(const int32_t& num);

  void SetDenominator(const int32_t& denom);

  Rational& operator+=(const Rational& rhs);

  Rational& operator-=(const Rational& rhs);

  Rational& operator*=(const Rational& rhs);

  Rational& operator/=(const Rational& rhs);

  Rational operator+() const;

  Rational operator-() const;

  Rational& operator++();

  Rational& operator--();

  Rational operator++(int);

  Rational operator--(int);

  friend bool operator<(const Rational& lhs, const Rational& rhs);

  friend std::istream& operator>>(std::istream& is, Rational& rhs);

  friend std::ostream& operator<<(std::ostream& os, const Rational& rhs);
};

Rational operator+(const Rational& lhs, const Rational& rhs);

Rational operator-(const Rational& lhs, const Rational& rhs);

Rational operator*(const Rational& lhs, const Rational& rhs);

Rational operator/(const Rational& lhs, const Rational& rhs);

bool operator<(const Rational& lhs, const Rational& rhs);

bool operator>(const Rational& lhs, const Rational& rhs);

bool operator==(const Rational& lhs, const Rational& rhs);

bool operator!=(const Rational& lhs, const Rational& rhs);

bool operator<=(const Rational& lhs, const Rational& rhs);

bool operator>=(const Rational& lhs, const Rational& rhs);
