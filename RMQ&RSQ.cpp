class FenwickTree {
 private:
  std::vector<int64_t> ft_;

  int64_t PrefixSum(int idx) {
    int64_t sum = 0;
    for (int i = idx; i >= 0; i = (i & (i + 1)) - 1) {
      sum += ft_[i];
    }
    return sum;
  }

 public:
  explicit FenwickTree(const std::vector<int> &v) {
    std::vector<int64_t> prefix_sum(v.size() + 1);
    prefix_sum[0] = 0;
    for (size_t i = 1; i <= v.size(); ++i) {
      prefix_sum[i] = prefix_sum[i - 1] + v[i - 1];
    }
    ft_.resize(v.size());
    for (size_t i = 0; i < v.size(); ++i) {
      ft_[i] = prefix_sum[i + 1] - prefix_sum[i & (i + 1)];
    }
  }

  void Add(size_t idx, int64_t delta) {
    for (size_t i = idx; i < ft_.size(); i = i | (i + 1)) {
      ft_[i] += delta;
    }
  }

  int64_t Query(int l, int r) {
    return PrefixSum(r) - PrefixSum(l - 1);
  }
};

class SparseTable {
 private:
  std::vector<std::vector<int>> table_;

 public:
  explicit SparseTable(const std::vector<int>& v) {
    size_t n = v.size();
    size_t log_n = std::__lg(n);
    table_.resize(log_n + 1);
    table_[0] = v;
    for (size_t k = 1; k <= log_n; ++k) {
      table_[k].resize(n - (1 << k) + 1);
      for (size_t i = 0; i <= n - (1 << k); ++i) {
        table_[k][i] = std::min(table_[k - 1][i], table_[k - 1][i + (1 << (k - 1))]);
      }
    }
  }

  int Min(size_t l, size_t r) {
    size_t log_n = std::__lg(r - l + 1);
    return std::min(table_[log_n][l], table_[log_n][r - (1 << log_n) + 1]);
  }
};
