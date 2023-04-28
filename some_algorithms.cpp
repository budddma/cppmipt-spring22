int LowerBound(const int* arr, int val, int n) {  // первый больше либо равынй
 int l = -1, r = n;   // чтобы границы вошли
 while (r - l > 1) {
   int mid = (l + r) / 2;
   if (arr[mid] < val){
     l = mid;
   } else {
     r = mid;
   }
 }
 return r;
}

int UpperBound(const int* arr, int val, int n) {  // первый больший
 int l = -1, r = n;   // чтобы границы вошли
 while (r - l > 1) {
   int mid = (l + r) / 2;
   if (arr[mid] <= val){
     l = mid;
   } else {
     r = mid;
   }
 }
 return r;
}

void Sieve(int k, int* prime) {
  auto* is_prime = new bool[k + 1];
  for (int i = 2; i <= k; ++i) {
    is_prime[i] = true;
  }
  for (int i = 2; i <= sqrt(k); ++i) {
    if (i == 2) {
      for (int j = i; j * i <= k; ++j) {
        is_prime[i * j] = false;
      }
    } else if (is_prime[i]) {
      for (int j = i; j * i <= k; j += 2) {
        is_prime[i * j] = false;
      }
    }
  }
  int counter = 0;
  for (int i = 2; i <= k; ++i) {
    if(is_prime[i]) {
      prime[counter++] = i;
    }
  }
  delete[] is_prime;
}
