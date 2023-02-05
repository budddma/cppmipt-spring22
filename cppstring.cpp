#include "cppstring.h"
#include <cstring>
#include <iostream>

void String::IncreaseCapacity(size_t new_size) {
  if (capacity_ == 0) {
    capacity_ = 1;
  }
  while (new_size > capacity_) {
    capacity_ *= 2;
  }
  auto* new_data = new char[capacity_];
  if (data_ != nullptr) {
    for (size_t i = 0; i < size_; ++i) {
      new_data[i] = data_[i];
    }
  }
  delete[] data_;
  data_ = new_data;
}

String::String() {
  data_ = nullptr;
  size_ = capacity_ = 0;
}

String::String(size_t size, char symbol) {
  if (size > capacity_) {
    IncreaseCapacity(size);
  }
  for (size_t i = 0; i < size; ++i) {
    data_[i] = symbol;
  }
  size_ = size;
}

String::String(const char* str) {
  size_t new_size = strlen(str);
  if (new_size > capacity_) {
    IncreaseCapacity(new_size);
  }
  for (size_t i = 0; i < new_size; ++i) {
    data_[i] = str[i];
  }
  size_ = new_size;
}

String::String(const char* str, size_t size) {
  if (size > capacity_) {
    IncreaseCapacity(size);
  }
  for (size_t i = 0; i < size; ++i) {
    data_[i] = str[i];
  }
  size_ = size;
}

String::String(const String& other) {
  if (other.size_ > capacity_) {
    IncreaseCapacity(other.size_);
  }
  for (size_t i = 0; i < other.size_; ++i) {
    data_[i] = other[i];
  }
  size_ = other.size_;
}

String& String::operator=(const String& other) {
  if (other.size_ > capacity_) {
    IncreaseCapacity(other.size_);
  }
  for (size_t i = 0; i < other.size_; ++i) {
    data_[i] = other[i];
  }
  size_ = other.size_;
  return *this;
}

String::~String() {
  delete[] data_;
}

char& String::operator[](size_t idx) {
  return data_[idx];
}

const char& String::operator[](size_t idx) const {
  return data_[idx];
}

char& String::At(size_t idx) {
  if (idx >= size_) {
    throw StringOutOfRange{};
  }
  return data_[idx];
}

const char& String::At(size_t idx) const {
  if (idx >= size_) {
    throw StringOutOfRange{};
  }
  return data_[idx];
}

char& String::Front() {
  return data_[0];
}

const char& String::Front() const {
  return data_[0];
}

char& String::Back() {
  return data_[size_ - 1];
}

const char& String::Back() const {
  return data_[size_ - 1];
}

char* String::CStr() {
  return data_;
}

const char* String::CStr() const {
  return data_;
}

char* String::Data() {
  return data_;
}

const char* String::Data() const {
  return data_;
}

bool String::Empty() const {
  return size_ == 0;
}

size_t String::Size() const {
  return size_;
}

size_t String::Length() const {
  return size_;
}

size_t String::Capacity() const {
  return capacity_;
}

void String::Clear() {
  size_ = 0;
}

void String::Swap(String& other) {
  std::swap(data_, other.data_);
  std::swap(capacity_, other.capacity_);
  std::swap(size_, other.size_);
}

void String::PopBack() {
  --size_;
}

void String::PushBack(char symbol) {
  if (size_ == capacity_) {
    IncreaseCapacity(size_ + 1);
  }
  data_[size_++] = symbol;
}

String& String::operator+=(const String& other) {
  if (size_ + other.size_ > capacity_) {
    IncreaseCapacity(size_ + other.size_);
  }
  size_t j = 0;
  for (size_t i = size_; i < size_ + other.size_; ++i) {
    data_[i] = other[j++];
  }
  size_ += other.size_;
  return *this;
}

void String::Resize(size_t new_size, char symbol) {
  if (new_size > size_) {
    if (new_size > capacity_) {
      IncreaseCapacity(new_size);
    }
    for (size_t i = size_; i < new_size; ++i) {
      data_[i] = symbol;
    }
    size_ = new_size;
  }
  size_ = new_size;
}

void String::Reserve(size_t new_capacity) {
  if (new_capacity > capacity_) {
    capacity_ = new_capacity;
    auto* new_data = new char[capacity_];
    for (size_t i = 0; i < size_; ++i) {
      new_data[i] = data_[i];
    }
    delete[] data_;
    data_ = new_data;
  }
}

void String::ShrinkToFit() {
  capacity_ = size_;
  auto* new_data = new char[capacity_];
  for (size_t i = 0; i < size_; ++i) {
    new_data[i] = data_[i];
  }
  delete[] data_;
  data_ = new_data;
}

String operator+(const String& lhs, const String& rhs) {
  String res = lhs;
  res += rhs;
  return res;
}

bool operator<(const String& lhs, const String& rhs) {
  for (size_t i = 0; i < std::min(lhs.size_, rhs.size_); ++i) {
    if (static_cast<size_t>(lhs[i]) < static_cast<size_t>(rhs[i])) {
      return true;
    }
    if (static_cast<size_t>(lhs[i]) > static_cast<size_t>(rhs[i])) {
      return false;
    }
  }
  return lhs.size_ < rhs.size_;
}

bool operator>(const String& lhs, const String& rhs) {
  return rhs < lhs;
}

bool operator==(const String& lhs, const String& rhs) {
  return !(rhs < lhs) && !(lhs < rhs);
}

bool operator!=(const String& lhs, const String& rhs) {
  return (rhs < lhs) || (lhs < rhs);
}

bool operator<=(const String& lhs, const String& rhs) {
  return !(rhs < lhs);
}

bool operator>=(const String& lhs, const String& rhs) {
  return !(lhs < rhs);
}

std::ostream& operator<<(std::ostream& os, const String& str) {
  for (size_t i = 0; i < str.size_; ++i) {
    os << str.data_[i];
  }
  return os;
}
