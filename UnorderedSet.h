#pragma once
#include <list>
#include <vector>

#define ITERATOR_IMPLEMENTED

template <class T>
class UnorderedSet {
 public:
  using Iterator = typename std::list<T>::const_iterator;
  using ConstIterator = typename std::list<T>::const_iterator;
  using DifferenceType = typename std::list<T>::difference_type;

  UnorderedSet() = default;

  UnorderedSet(size_t count) {  // NOLINT
    buckets_.resize(count, Iterator{});
  }

  template <class Iter, class = std::enable_if_t<std::is_base_of_v<
                            std::forward_iterator_tag, typename std::iterator_traits<Iter>::iterator_category>>>
  UnorderedSet(Iter first, Iter last) {
    size_t bucket_count = std::distance(first, last);
    if (bucket_count != 0) {
      buckets_.resize(bucket_count, Iterator{});
      for (; first != last; ++first) {
        size_t b_idx = Bucket(*first);
        if (buckets_[b_idx] != Iterator{}) {
          list_.insert(buckets_[b_idx], *first);
          --buckets_[b_idx];
        } else {
          list_.emplace_front(*first);
          buckets_[b_idx] = list_.begin();
        }
      }
    }
  }

  UnorderedSet(const UnorderedSet& other) {
    list_ = other.list_;
    buckets_ = other.buckets_;
  }

  UnorderedSet(UnorderedSet&& other) noexcept {
    list_ = std::move(other.list_);
    buckets_ = std::move(other.buckets_);
  }

  void Swap(UnorderedSet& other) {
    std::swap(list_, other.list_);
    std::swap(buckets_, other.buckets_);
  }

  UnorderedSet& operator=(const UnorderedSet& other) {
    if (this != &other) {
      UnorderedSet(other).Swap(*this);
    }
    return *this;
  }

  UnorderedSet& operator=(UnorderedSet&& other) noexcept {
    if (this != &other) {
      UnorderedSet(std::move(other)).Swap(*this);
    }
    return *this;
  }

  size_t Size() const {
    return list_.size();
  }

  bool Empty() const {
    return Size() == 0;
  }

  void Clear() {
    list_.clear();
  }

  void Insert(const T& key) {
    if (Find(key)) {
      return;
    }
    if (Size() >= BucketCount()) {
      size_t new_bucket_count = BucketCount() == 0 ? 1 : BucketCount() * 2;
      Reserve(new_bucket_count);
    }
    size_t b_idx = Bucket(key);
    if (buckets_[b_idx] != Iterator{}) {
      list_.insert(buckets_[b_idx], key);
      --buckets_[b_idx];
    } else {
      list_.emplace_front(key);
      buckets_[b_idx] = list_.begin();
    }
  }

  void Insert(T&& key) noexcept {
    if (Find(key)) {
      return;
    }
    if (Size() >= BucketCount()) {
      size_t new_bucket_count = BucketCount() == 0 ? 1 : BucketCount() * 2;
      Reserve(new_bucket_count);
    }
    size_t b_idx = Bucket(key);
    if (buckets_[b_idx] != Iterator{}) {
      list_.insert(buckets_[b_idx], std::move(key));
      --buckets_[b_idx];
    } else {
      list_.emplace_front(key);
      buckets_[b_idx] = list_.begin();
    }
  }

  void Erase(const T& key) {
    if (Find(key)) {
      size_t b_idx = Bucket(key);
      auto next_it = Iterator{};
      if (BucketSize(b_idx) != 1) {
        next_it = std::next(buckets_[b_idx]);
      }
      auto it = buckets_[b_idx];
      while (*it != key) {
        ++it;
      }
      if (buckets_[b_idx] == it) {
        buckets_[b_idx] = next_it;
      }
      list_.erase(it);
    }
  }

  bool Find(const T& key) const {
    if (Empty()) {
      return false;
    }
    size_t key_bucket = Bucket(key);
    auto it = buckets_[key_bucket];
    if (it != Iterator{}) {
      size_t b_size = BucketSize(key_bucket);
      for (size_t i = 0; i < b_size; ++it, ++i) {
        if (*it == key) {
          return true;
        }
      }
    }
    return false;
  }

  void Rehash(size_t new_bucket_count) {
    if (new_bucket_count >= Size() && new_bucket_count != BucketCount()) {
      std::list<T> new_list;
      std::vector<Iterator> new_buckets(new_bucket_count, Iterator{});
      if (!Empty()) {
        std::hash<T> hash{};
        auto it = list_.begin();
        size_t size = Size();
        for (size_t i = 0; i < size; ++i) {
          size_t b_idx = hash(*it) % new_bucket_count;
          if (new_buckets[b_idx] != Iterator{}) {
            auto next_it = std::next(it);
            new_list.splice(new_buckets[b_idx], list_, it);
            --new_buckets[b_idx];
            it = next_it;
          } else {
            auto next_it = std::next(it);
            new_list.splice(new_list.begin(), list_, it);
            new_buckets[b_idx] = new_list.begin();
            it = next_it;
          }
        }
      }
      list_ = std::move(new_list);
      buckets_ = std::move(new_buckets);
    }
  }

  void Reserve(size_t new_bucket_count) {
    if (new_bucket_count > BucketCount()) {
      Rehash(new_bucket_count);
    }
  }

  size_t BucketCount() const {
    return buckets_.size();
  }

  size_t BucketSize(size_t id) const {
    if (Empty() || BucketCount() <= id) {
      return 0u;
    }
    size_t b_size = Bucket(list_.back()) == id;
    for (Iterator it = buckets_[id]; *it != list_.back() && Bucket(*it) == id; ++it) {
      ++b_size;
    }
    return b_size;
  }

  size_t Bucket(const T& key) const {
    std::hash<T> hash{};
    return hash(key) % BucketCount();
  }

  float LoadFactor() const {
    return BucketCount() != 0 ? 1.0f * Size() / BucketCount() : 0.0f;
  }

  Iterator begin() {  //  NOLINT
    return list_.begin();
  }

  Iterator end() {  //  NOLINT
    return list_.end();
  }

  ConstIterator begin() const {  //  NOLINT
    return list_.begin();
  }

  ConstIterator end() const {  //  NOLINT
    return list_.end();
  }

  ConstIterator cbegin() const {  //  NOLINT
    return begin();
  }

  ConstIterator cend() const {  //  NOLINT
    return end();
  }

 private:
  std::list<T> list_;
  std::vector<Iterator> buckets_;
};
