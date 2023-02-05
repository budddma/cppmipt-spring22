// stack

struct StNode{
  int key = 0;
  StNode* next = nullptr;
};

struct Stack{
  int size = 0;
  StNode* top = nullptr;
};

void Push(Stack& S, int val){
  S.top = new StNode{val, S.top};
  ++S.size;
}

void Pop(Stack& S) {
  if (S.size != 0) {
    StNode* temp = S.top;
    S.top = S.top->next;
    delete temp;
    --S.size;
  }
}

void Clear(Stack& S) {
  while (S.size != 0) {
    Pop(S);
  }
}

// queue

struct QuNode{
  int key = 0;
  QuNode* prev = nullptr; 
};

struct Queue{
  int size = 0;
  QuNode* start = nullptr;
  QuNode* end = nullptr;
};

void Push(Queue& Q, int val) {
  if (Q.size == 0) {
    Q.end = Q.start = new QuNode{val};
    ++Q.size;
    return;
  }
  Q.start = Q.start->prev = new QuNode{val};
  ++Q.size;
}

void Pop(Queue& Q) {
  if (Q.size != 0) {
    QuNode* temp = Q.end;
    Q.end = Q.end->prev;
    delete Q.end;
    --Q.size;
  }
}

void Clear(Queue& Q) {
  while (Q.size != 0) {
    Pop(Q);
  }
}

// deque

struct DeqNode {
  int key = 0;
  DeqNode* next = nullptr;
  DeqNode* prev = nullptr;
};

struct Deque{
  int size = 0;
  DeqNode* start = nullptr;
  DeqNode* end = nullptr;
};

void PushFront(Deque& D, int val) {
  if (D.size == 0) {
    D.end = D.start = new DeqNode{val};
    ++D.size;
    return;
  }
  D.start = D.start->next = new DeqNode{val, nullptr, D.start};
  ++D.size;
}

void PushBack(Deque& D, int val) {
  if (D.size == 0) {
    D.start = D.end = new DeqNode{val};
    ++D.size;
    return;
  }
  D.end = D.end->prev = new DeqNode{val, D.end, nullptr};
  ++D.size;
}

void PopFront(Deque& D) {
  if (D.size != 0) {
    DeqNode* temp = D.start;
    D.start = D.start->prev;
    delete temp;
    --D.size;
  }
}

void PopBack(Deque& D) {
  if (D.size != 0) {
    DeqNode* temp = D.end;
    D.end = D.end->next;
    delete temp;
    --D.size;
  }
}

void Clear(Deque& D) {
  while (D.size != 0) {
    PopFront(D);
  }
}

