#pragma once
#include <stdexcept>

class StringOutOfRange : public std::out_of_range {
 public:
  StringOutOfRange() : std::out_of_range("StringOutOfRange") {
  }
};

class String {
 private:
  char* data_ = nullptr;
  size_t capacity_ = 0;
  size_t size_ = 0;

  void IncreaseCapacity(size_t new_size);

 public:
  String();

  String(size_t size, char symbol);

  String(const char* str);  // NOLINT

  String(const char* str, size_t size);

  String(const String& other);

  String& operator=(const String& other);

  ~String();

  char& operator[](size_t idx);

  const char& operator[](size_t idx) const;

  char& At(size_t idx);

  const char& At(size_t idx) const;

  char& Front();

  const char& Front() const;

  char& Back();

  const char& Back() const;

  char* CStr();

  const char* CStr() const;

  char* Data();

  const char* Data() const;

  bool Empty() const;

  size_t Size() const;

  size_t Length() const;

  size_t Capacity() const;

  void Clear();

  void Swap(String& other);

  void PopBack();

  void PushBack(char symbol);

  String& operator+=(const String& other);

  void Resize(size_t new_size, char symbol);

  void Reserve(size_t new_capacity);

  void ShrinkToFit();

  friend bool operator<(const String& lhs, const String& rhs);

  friend std::ostream& operator<<(std::ostream& os, const String& str);
};

String operator+(const String& lhs, const String& rhs);

bool operator>(const String& lhs, const String& rhs);

bool operator<=(const String& lhs, const String& rhs);

bool operator>=(const String& lhs, const String& rhs);

bool operator==(const String& lhs, const String& rhs);

bool operator!=(const String& lhs, const String& rhs);