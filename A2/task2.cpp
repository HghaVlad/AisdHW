#include <iostream>
#include <vector>

void insertionSort(std::vector<int> &arr, int left, int right) {
  for (int i = left + 1; i <= right; i++) {
    int key = arr[i];
    int j = i - 1;
    while (j >= left && arr[j] > key) {
      arr[j + 1] = arr[j];
      j--;
    }
    arr[j + 1] = key;
  }
}

void merge(std::vector<int> &arr, int left, int mid, int right) {
  int n1 = mid - left + 1;
  int n2 = right - mid;

  std::vector<int> leftArr(n1), rightArr(n2);

  for (int i = 0; i < n1; i++)
    leftArr[i] = arr[left + i];
  for (int j = 0; j < n2; j++)
    rightArr[j] = arr[mid + 1 + j];

  int i = 0, j = 0, k = left;

  while (i < n1 && j < n2) {
    if (leftArr[i] <= rightArr[j]) {
      arr[k] = leftArr[i];
      i++;
    } else {
      arr[k] = rightArr[j];
      j++;
    }
    k++;
  }

  while (i < n1) {
    arr[k] = leftArr[i];
    i++;
    k++;
  }

  while (j < n2) {
    arr[k] = rightArr[j];
    j++;
    k++;
  }
}

void hybridMergeSort(std::vector<int> &arr, int left, int right, int threshold) {
  if (left >= right) return;

  if (right - left + 1 <= threshold) {
    insertionSort(arr, left, right);
  } else {
    int mid = left + (right - left) / 2;
    hybridMergeSort(arr, left, mid, threshold);
    hybridMergeSort(arr, mid + 1, right, threshold);
    merge(arr, left, mid, right);
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n;
  std::cin >> n;
  std::vector<int> arr(n);

  for (int i = 0; i < n; i++) {
    std::cin >> arr[i];
  }

  if (n > 0) {
    hybridMergeSort(arr, 0, n - 1, 15);
  }

  for (int i = 0; i < n; i++) {
    std::cout << arr[i] << " ";
  }

  return 0;
}