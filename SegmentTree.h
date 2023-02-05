#include <iostream>
#include <vector>
#include <algorithm>

const int kNeutral = std::numeric_limits<int>::min();

class SegmentTree {
 private:
  struct Node {
    int value = kNeutral;
    int promise = 0;
  };

  std::vector<Node> tree_;

  size_t Left(size_t parent) {
    return 2 * parent + 1;
  }

  size_t Right(size_t parent) {
    return 2 * parent + 2;
  }

  void Push(size_t idx) {
    if (tree_[idx].value == kNeutral) {
      tree_[idx].value = 0;
    }
    tree_[idx].value += tree_[idx].promise;
    if (2 * idx + 2 < tree_.size()) {
      tree_[Left(idx)].promise += tree_[idx].promise;
      tree_[Right(idx)].promise += tree_[idx].promise;
    }
    tree_[idx].promise = 0;
  }

  int Max(size_t l, size_t r, size_t idx, size_t l_idx, size_t r_idx) {
    if (idx >= tree_.size() || (l > r_idx || l_idx > r)) {
      return kNeutral;
    }
    if (l <= l_idx && r_idx <= r) {
      return tree_[idx].value + tree_[idx].promise;
    }
    Push(idx);
    size_t mid_idx = (l_idx + r_idx) / 2;
    return std::max(Max(l, r, Left(idx), l_idx, mid_idx), Max(l, r, Right(idx), mid_idx + 1, r_idx));
  }

  void Add(size_t l, size_t r, size_t idx, size_t l_idx, size_t r_idx, int delta) {
    if (idx >= tree_.size() || (l > r_idx || l_idx > r)) {
      return;
    }
    if (l <= l_idx && r_idx <= r) {
      tree_[idx].promise += delta;
      return;
    }
    Push(idx);
    size_t mid_idx = (l_idx + r_idx) / 2;
    Add(l, r, Left(idx), l_idx, mid_idx, delta);
    Add(l, r, Right(idx), mid_idx + 1, r_idx, delta);
    tree_[idx].value = std::max(tree_[Left(idx)].value + tree_[Left(idx)].promise,
                                tree_[Right(idx)].value + tree_[Right(idx)].promise);
  }

 public:
  explicit SegmentTree(const std::vector<int> &v) {
    auto n = static_cast<int>(v.size());
    size_t log_n = std::__lg(n);
    if (n != (1 << log_n)) {
      n = 1 << (log_n + 1);
    }
    tree_.resize(2 * n - 1);
    for (size_t i = 0; i < v.size(); ++i) {
      tree_[i + n - 1].value = v[i];
    }
    for (int i = n - 2; i >= 0; --i) {
      tree_[i].value = std::max(tree_[Left(i)].value, tree_[Right(i)].value);
    }
  }

  int Max(size_t l, size_t r) {
    return Max(l, r, 0, 0, tree_.size() / 2);
  }

  void Add(size_t l, size_t r, int delta) {
    Add(l, r, 0, 0, tree_.size() / 2, delta);
  }
};