#pragma once
#include <stdexcept>

#define WEAK_PTR_IMPLEMENTED

class BadWeakPtr : public std::runtime_error {
 public:
  BadWeakPtr() : std::runtime_error("BadWeakPtr") {
  }
};

struct Counter {
  size_t strong = 0;
  size_t weak = 0;
};

template <class T>
class WeakPtr;

template <class T>
class SharedPtr {
 private:
  T* ptr_;
  Counter* cnt_;

 public:
  SharedPtr() : ptr_(nullptr), cnt_(nullptr) {
  }

  SharedPtr(T* ptr) : ptr_(ptr), cnt_(nullptr) {  // NOLINT
    if (ptr != nullptr) {
      cnt_ = new Counter;
      cnt_->strong = 1;
    }
  }

  SharedPtr(const SharedPtr<T>& other) : ptr_(other.ptr_), cnt_(other.cnt_) {
    if (cnt_ != nullptr) {
      ++cnt_->strong;
    }
  }

  explicit SharedPtr(const WeakPtr<T>& other) {
    if (other.Expired()) {
      throw BadWeakPtr();
    }
    ptr_ = other.ptr_;
    cnt_ = other.cnt_;
    ++cnt_->strong;
  }

  SharedPtr& operator=(const SharedPtr& other) {
    if (this != &other) {
      if (ptr_ != nullptr) {
        if (--cnt_->strong == 0) {
          delete ptr_;
          if (cnt_->weak == 0) {
            delete cnt_;
          }
        }
      }
      ptr_ = other.ptr_;
      cnt_ = other.cnt_;
      if (cnt_ != nullptr) {
        ++cnt_->strong;
      }
    }
    return *this;
  }

  SharedPtr(SharedPtr&& other) noexcept : ptr_(other.ptr_), cnt_(other.cnt_) {
    other.ptr_ = nullptr;
    other.cnt_ = nullptr;
  }

  SharedPtr& operator=(SharedPtr&& other) noexcept {
    if (this != &other) {
      if (ptr_ != nullptr) {
        if (--cnt_->strong == 0) {
          delete ptr_;
          if (cnt_->weak == 0) {
            delete cnt_;
          }
        }
      }
      ptr_ = other.ptr_;
      cnt_ = other.cnt_;
      other.ptr_ = nullptr;
      other.cnt_ = nullptr;
    }
    return *this;
  }

  void Reset(T* ptr = nullptr) {
    if (ptr_ != nullptr) {
      if (--cnt_->strong == 0) {
        delete ptr_;
        if (cnt_->weak == 0) {
          delete cnt_;
        }
      }
    }
    ptr_ = ptr;
    cnt_ = nullptr;
    if (ptr != nullptr) {
      cnt_ = new Counter;
      cnt_->strong = 1;
    }
  }

  void Swap(SharedPtr& other) {
    std::swap(ptr_, other.ptr_);
    std::swap(cnt_, other.cnt_);
  }

  T* Get() const {
    return ptr_;
  }

  size_t UseCount() const {
    if (ptr_ == nullptr) {
      return 0;
    }
    return cnt_->strong;
  }

  T& operator*() const {
    return *ptr_;
  }

  T* operator->() const {
    return ptr_;
  }

  explicit operator bool() const {
    return ptr_ != nullptr;
  }

  friend class WeakPtr<T>;

  ~SharedPtr() {
    if (ptr_ != nullptr) {
      if (--cnt_->strong == 0) {
        delete ptr_;
        if (cnt_->weak == 0) {
          delete cnt_;
        }
      }
      ptr_ = nullptr;
      cnt_ = nullptr;
    }
  }
};

template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args) {
  return SharedPtr<T>{new T(std::forward<Args>(args)...)};
}

template <typename T>
class WeakPtr {
 private:
  T* ptr_;
  Counter* cnt_;

 public:
  friend class SharedPtr<T>;

  WeakPtr() : ptr_(nullptr), cnt_(nullptr) {
  }

  WeakPtr(const WeakPtr<T>& other) : ptr_(other.ptr_), cnt_(other.cnt_) {
    if (cnt_ != nullptr) {
      ++cnt_->weak;
    }
  }

  WeakPtr(WeakPtr<T>&& other) : ptr_(other.ptr_), cnt_(other.cnt_) {
    other.cnt_ = nullptr;
    other.ptr_ = nullptr;
  }

  WeakPtr& operator=(const WeakPtr<T>& other) {
    if (this != &other) {
      if (ptr_ != nullptr) {
        --cnt_->weak;
      }

      ptr_ = other.ptr_;
      cnt_ = other.cnt_;
      if (ptr_ != nullptr) {
        ++cnt_->weak;
      }
    }
    return *this;
  }

  WeakPtr& operator=(SharedPtr<T>&& other) noexcept {
    if (this != &other) {
      if (ptr_ != nullptr) {
        --cnt_->weak;
      }
      ptr_ = other.ptr_;
      cnt_ = other.cnt_;
      other.ptr_ = nullptr;
      other.cnt_ = nullptr;
    }
    return *this;
  }

  WeakPtr(const SharedPtr<T>& ptr) {  // NOLINT
    if (ptr.ptr_ != nullptr) {
      ptr_ = ptr.ptr_;
      cnt_ = ptr.cnt_;
      ++cnt_->weak;
    } else {
      ptr_ = nullptr;
      cnt_ = nullptr;
    }
  }
  void Swap(WeakPtr<T>& lhs, WeakPtr<T>& rhs) {
    std::swap(lhs.cnt_, rhs.cnt_);
    std::swap(lhs.ptr_, rhs.ptr_);
  }

  void Reset() {
    if (ptr_ != nullptr) {
      --cnt_->weak;
    }
    ptr_ = nullptr;
    cnt_ = nullptr;
  }

  size_t UseCount() const {
    if (ptr_ == nullptr) {
      return 0;
    }
    return cnt_->strong;
  }

  bool Expired() const {
    if (cnt_ == nullptr) {
      return true;
    }
    return cnt_->strong == 0;
  }

  SharedPtr<T> Lock() const {
    if (Expired()) {
      return nullptr;
    }
    return SharedPtr<T>(*this);
  }

  ~WeakPtr() {
    if (ptr_ != nullptr) {
      --cnt_->weak;
      if (cnt_->strong == 0) {
        if (cnt_->weak == 0) {
          delete cnt_;
        }
        cnt_ = nullptr;
      }
    }
  }
};
