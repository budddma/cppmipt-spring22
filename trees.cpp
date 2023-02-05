// binary search

struct Node {
  int key = 0;
  int depth = 0;
  Node* par = nullptr;
  Node* left = nullptr;
  Node* right = nullptr;
};

struct BinSearchTree {
  int height = 0;
  Node* root = nullptr;
};

void Insert(BinSearchTree& T, Node* v, Node* par, int val) {
  if (v == nullptr) {
    if (par == nullptr) {
      T.root = new Node{val};
      return;
    }
    v = new Node{val, par->depth + 1, par};
    if (v->key < par->key) {
      par->left = v;
    } else {
      par->right = v;
    }
    T.height = std::max(T.height, v->depth);
    return;
  }
  if (v->key > val) {
    Insert(T, v->left, v, val);
  } else if (v->key < val) {
    Insert(T, v->right, v, val);
  }
}

void Clear(Node* v) {
  if (v == nullptr) {
    return;
  }
  if (v->left != nullptr) {
    Clear(v->left);
  }
  if (v->right != nullptr) {
    Clear(v->right);
  }
  delete v;
}

// heap

void SiftDown(int* heap, int i, int size) {
 int left = 2 * i + 1;
 int right = 2 * i + 2;
 int largest = i;
 if (left < size && heap[left] > heap[largest]) {
   largest = left;
 }
 if (right < size && heap[right] > heap[largest]) {
   largest = right;
 }
 if (largest != i) {
   std::swap(heap[largest], heap[i]);
   SiftDown(heap, largest, size);
 }
}

void SiftUp(int* heap, int i) { 
  int parent = (i - 1) / 2;
  if (i > 0 && heap[parent] < heap[i]) {
    std::swap(heap[i], heap[parent]);
    SiftUp(heap, parent);
  }
}

void BuildHeap(int* heap, int size) {
 for (int i = size / 2; i >= 0; --i) {
   SiftDown(heap, i, size);
 }
}
