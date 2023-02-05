#include <iostream>
#include <memory>

struct Node {
  int x;
  int y;
  int value;
  int64_t result = 0;
  int promise = 0;
  int size = 1;
  Node* parent = nullptr;
  Node* left = nullptr;
  Node* right = nullptr;
};

int Size(Node* node_ptr) {
  return (node_ptr == nullptr) ? 0 : node_ptr->size;
}

int64_t Result(Node* node_ptr) {
  return (node_ptr == nullptr) ? 0 : node_ptr->result;
}

void Push(Node* node_ptr) {
  if (node_ptr != nullptr) {
    node_ptr->value += node_ptr->promise;
    node_ptr->result += node_ptr->promise;
    if (node_ptr->left != nullptr) {
      node_ptr->left->promise += node_ptr->promise;
    }
    if (node_ptr->right != nullptr) {
      node_ptr->right->promise += node_ptr->promise;
    }
    node_ptr->promise = 0;
  }
}

void FixNode(Node* node_ptr) {
  if (node_ptr != nullptr) {
    node_ptr->result = Result(node_ptr->left) + node_ptr->result + Result(node_ptr->right);
    node_ptr->size = Size(node_ptr->left) + 1 + Size(node_ptr->right);
  }
}

Node* Merge(Node* left_root, Node* right_root) {
  if (left_root == nullptr || right_root == nullptr) {
    return left_root == nullptr ? right_root : left_root;
  }
  Push(left_root);
  if (left_root->y < right_root->y) {
    left_root->right = Merge(left_root->right, right_root);
    if (left_root->right != nullptr) {
      left_root->right->parent = left_root;
    }
    FixNode(left_root);
    return left_root;
  }
  Push(right_root);
  right_root->left = Merge(left_root, right_root->left);
  if (right_root->left != nullptr) {
    right_root->left->parent = right_root;
  }
  FixNode(right_root);
  return right_root;
}

std::tuple<Node*, Node*> Split(Node* root, int k) {
  if (root == nullptr) {
    return {nullptr, nullptr};
  }
  Push(root);
  if (Size(root->left) < k) {
    auto[left, right] = Split(root->right, k);
    root->right = left;
    if (left != nullptr) {
      left->parent = root;
    }
    if (right != nullptr) {
      right->parent = nullptr;
    }
    FixNode(root);
    return {root, right};
  }
  auto[left, right] = Split(root->left, k);
  root->left = right;
  if (right != nullptr) {
    right->parent = root;
  }
  if (left != nullptr) {
    left->parent = nullptr;
  }
  FixNode(root);
  return {left, root};
}

Node* Find(Node* root, int key) {
  if (root == nullptr || key == root->x) {
    return root;
  }
  if (key < root->x) {
    return Find(root->left, key);
  }
  return Find(root->right, key);
}

Node* LowerBound(Node* root, int key) {
  if (root == nullptr || key == root->x) {
    return root;
  }
  if (root->x < key) {
    return LowerBound(root->right, key);
  }
  auto lb = LowerBound(root->left, key);
  if (lb == nullptr) {
    return root;
  }
  return lb;
}

Node* NotMore(Node* root, int key) {
  if (root == nullptr || key == root->x) {
    return root;
  }
  if (root->x > key) {
    return NotMore(root->left, key);
  }
  auto nm = NotMore(root->right, key);
  if (nm == nullptr) {
    return root;
  }
  return nm;
}

void Clear(Node* root) {
  if (root != nullptr) {
    Clear(root->left);
    Clear(root->right);
    delete root;
  }
}

int Min(Node* root) {
  if (root->left == nullptr) {
    return root->x;
  }
  return Min(root->left);
}

int Max(Node* root) {
  if (root->right == nullptr) {
    return root->x;
  }
  return Max(root->right);
}

class CartesianTree {
 private:
  Node* root_ = nullptr;

 public:
  CartesianTree() = default;

  int64_t Sum(int begin, int end) {  // не включая end
    if (root_ == nullptr || end <= Min(root_) || Max(root_) < begin) {
      return 0;
    }
    auto[begin_left, begin_right] = Split(root_, begin);
    auto[sub_root, root_right] = Split(begin_right, end);
    auto sum = Result(sub_root);
    root_ = Merge(Merge(begin_left, sub_root), root_right);
    return sum;
  }

  void Add(int begin, int end, int delta) {  // не включая end
    if (root_ == nullptr || end <= Min(root_) || Max(root_) < begin) {
      return;
    }
    auto[begin_left, begin_right] = Split(root_, begin);
    auto[sub_root, root_right] = Split(begin_right, end);
    sub_root->promise += delta;
    root_ = Merge(Merge(begin_left, sub_root), root_right);
  }

  void Insert(int k, int y) {
    if (Find(root_, k) == nullptr) {
      auto node_ptr = new Node{k, y};
      auto[left, right] = Split(root_, k);
      root_ = Merge(Merge(left, node_ptr), right);
    }
  }

  void Erase(int k) {
    if (Find(root_, k) != nullptr) {
      auto[less, not_less] = Split(root_, k);
      auto[k_ptr, more] = Split(not_less, k + 1);
      delete k_ptr;
      root_ = Merge(less, more);
    }
  }

  std::string IsExist(int key) {
    return (Find(root_, key) == nullptr) ? "false" : "true";
  }

  std::string Next(int key) {
    auto next = LowerBound(root_, key + 1);
    if (next != nullptr) {
      return std::to_string(next->x);
    }
    return "none";
  }

  std::string Prev(int key) {
    auto prev = NotMore(root_, key - 1);
    if (prev != nullptr) {
      return std::to_string(prev->x);
    }
    return "none";
  }

  std::string KthElement(int k) {
    auto curr_root = root_;
    while (curr_root != nullptr) {
      if (Size(curr_root->left) == k) {
        return std::to_string(curr_root->x);
      }
      if (Size(curr_root->left) < k) {
        k -= Size(curr_root->left) + 1;
        curr_root = curr_root->right;
      } else {
        curr_root = curr_root->left;
      }
    }
    return "none";
  }

  ~CartesianTree() {
    Clear(root_);
  }
};