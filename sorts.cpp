// quick

int Partition(int* arr, int left, int right) {
  int pivot_val = arr[(left + right) / 2];
  int i = left, j = right;
  while (i <= j) {
    while (arr[i] < pivot_val) {
      ++i;
    }
    while (arr[j] > pivot_val) {
      --j;
    }
    if (i >= j) {
      break;
    }
    std::swap(arr[i++], arr[j--]);
  }
  return j;
}

void QuickSort(int* arr, int left, int right) {
  if (left < right) {
    int pivot = Partition(arr, left, right);
    QuickSort(arr, left, pivot);        
    QuickSort(arr, pivot + 1, right);    // правая граница включается
  }
}

// merge

void Merge(int* arr, int left, int right) {
  int mid = (left + right) / 2;
  int i = 0, j = 0;
  auto* res = new int[right - left];
  while (left + i < mid && mid + j < right) {
    if (arr[left + i] < arr[mid + j]) {
      res[i++ + j] = arr[left + i];
    } else {
      res[i + j++] = arr[mid + j];
    }
  }
  while (left + i < mid) {
    res[i++ + j] = arr[left + i];
  }
  while (mid + j < right) {
    res[i + j++] = arr[mid + j];
  }
  for (int i = 0; i < right - left; ++i) {
    arr[left + i] = res[i];
  }
  delete[] res;
}

void MergeSort(int* arr, int left, int right) {
  if (right - left > 1) {
    int mid = (left + right) / 2;
    MergeSort(arr, left, mid);
    MergeSort(arr, mid, right);   // правая граница не включается
    Merge(arr, left, right);
  }
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

void BuildHeap(int* heap, int size) {
 for (int i = size / 2; i >= 0; --i) {
   SiftDown(heap, i, size);
 }
}

void HeapSort(int* heap, int size) {
 BuildHeap(heap, size);
 while (size > 1) {
   std::swap(heap[0], heap[size - 1]);
   SiftDown(heap, 0, --size);
 }
}
