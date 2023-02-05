#pragma once
#include <iostream>
#include <stdexcept>

class MatrixIsDegenerateError : public std::runtime_error {
 public:
  MatrixIsDegenerateError() : std::runtime_error("MatrixIsDegenerateError") {
  }
};

class MatrixOutOfRange : public std::out_of_range {
 public:
  MatrixOutOfRange() : std::out_of_range("MatrixOutOfRange") {
  }
};

template <typename T, size_t R, size_t C>
class Matrix {
 public:
  T arr[R][C];

  size_t RowsNumber() const {
    return R;
  }

  size_t ColumnsNumber() const {
    return C;
  }

  T& operator()(size_t i, size_t j) {
    return arr[i][j];
  }

  const T& operator()(size_t i, size_t j) const {
    return arr[i][j];
  }

  T& At(size_t i, size_t j) {
    if (i >= R || j >= C) {
      throw MatrixOutOfRange{};
    }
    return arr[i][j];
  }

  const T& At(size_t i, size_t j) const {
    if (i >= R || j >= C) {
      throw MatrixOutOfRange{};
    }
    return arr[i][j];
  }

  Matrix& operator+=(const Matrix<T, R, C>& other) {
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        arr[i][j] += other(i, j);
      }
    }
    return *this;
  }

  Matrix& operator-=(const Matrix<T, R, C>& other) {
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        arr[i][j] -= other(i, j);
      }
    }
    return *this;
  }

  Matrix& operator*=(const Matrix<T, C, C>& other) {
    for (size_t i = 0; i < R; ++i) {
      T temp[C]{};
      std::swap(temp, arr[i]);
      for (size_t j = 0; j < C; ++j) {
        for (size_t k = 0; k < C; ++k) {
          arr[i][j] += temp[k] * other(k, j);
        }
      }
    }
    return *this;
  }

  template <typename U>
  Matrix& operator*=(const U& num) {
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        arr[i][j] *= num;
      }
    }
    return *this;
  }

  template <typename U>
  Matrix& operator/=(const U& num) {
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        arr[i][j] /= num;
      }
    }
    return *this;
  }
};

template <typename T, size_t R, size_t C>
Matrix<T, C, R> GetTransposed(const Matrix<T, R, C>& mat) {
  Matrix<T, C, R> transposed;
  for (size_t i = 0; i < R; ++i) {
    for (size_t j = 0; j < C; ++j) {
      transposed(j, i) = mat(i, j);
    }
  }
  return transposed;
}

template <typename T, size_t R, size_t C>
Matrix<T, R, C> operator+(const Matrix<T, R, C>& lhs, const Matrix<T, R, C>& rhs) {
  Matrix<T, R, C> res = lhs;
  res += rhs;
  return res;
}

template <typename T, size_t R, size_t C>
Matrix<T, R, C> operator-(const Matrix<T, R, C>& lhs, const Matrix<T, R, C>& rhs) {
  Matrix<T, R, C> res = lhs;
  res -= rhs;
  return res;
}

template <typename T, size_t R, size_t C, size_t A>
Matrix<T, R, A> operator*(const Matrix<T, R, C>& lhs, const Matrix<T, C, A>& rhs) {
  Matrix<T, R, A> res{};
  for (size_t i = 0; i < R; ++i) {
    for (size_t j = 0; j < A; ++j) {
      for (size_t k = 0; k < C; ++k) {
        res(i, j) += lhs(i, k) * rhs(k, j);
      }
    }
  }
  return res;
}

template <typename T, size_t R, size_t C, typename U>
Matrix<T, R, C> operator*(const Matrix<T, R, C>& mat, const U& num) {
  Matrix<T, R, C> res = mat;
  res *= num;
  return res;
}

template <typename T, size_t R, size_t C, typename U>
Matrix<T, R, C> operator*(const U& num, const Matrix<T, R, C>& mat) {
  Matrix<T, R, C> res = mat;
  res *= num;
  return res;
}

template <typename T, size_t R, size_t C, typename U>
Matrix<T, R, C> operator/(const Matrix<T, R, C>& mat, const U& num) {
  Matrix<T, R, C> res = mat;
  res /= num;
  return res;
}

template <typename T, size_t R, size_t C, typename U>
Matrix<T, R, C> operator/(const U& num, const Matrix<T, R, C>& mat) {
  Matrix<T, R, C> res = mat;
  res /= num;
  return res;
}

template <typename T, size_t R, size_t C>
bool operator==(const Matrix<T, R, C>& lhs, const Matrix<T, R, C>& rhs) {
  for (size_t i = 0; i < R; ++i) {
    for (size_t j = 0; j < C; ++j) {
      if (lhs(i, j) != rhs(i, j)) {
        return false;
      }
    }
  }
  return true;
}

template <typename T, size_t R, size_t C>
bool operator!=(const Matrix<T, R, C>& lhs, const Matrix<T, R, C>& rhs) {
  return !(lhs == rhs);
}

template <typename T, size_t R, size_t C>
std::istream& operator>>(std::istream& is, Matrix<T, R, C>& mat) {
  for (size_t i = 0; i < R; ++i) {
    for (size_t j = 0; j < C; ++j) {
      is >> mat(i, j);
    }
  }
  return is;
}

template <typename T, size_t R, size_t C>
std::ostream& operator<<(std::ostream& os, const Matrix<T, R, C>& mat) {
  for (size_t i = 0; i < R; ++i) {
    for (size_t j = 0; j < C; ++j) {
      os << mat(i, j);
      if (j != C - 1) {
        os << ' ';
      }
    }
    os << '\n';
  }
  return os;
}