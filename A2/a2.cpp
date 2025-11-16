#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

class ArrayGenerator {
 private:
  std::mt19937 gen;
  std::uniform_int_distribution<int> dist;

 public:
  ArrayGenerator(int minVal = 0, int maxVal = 6000) : gen(490290), dist(minVal, maxVal) {}

  std::vector<int> generateRandom(int size) {
    std::vector<int> arr(size);
    for (int i = 0; i < size; i++) {
      arr[i] = dist(gen);
    }
    return arr;
  }

  std::vector<int> generateReversed(int size) {
    std::vector<int> arr = generateRandom(size);
    std::sort(arr.begin(), arr.end(), std::greater<int>());
    return arr;
  }

  std::vector<int> generateAlmostSorted(int size, int numSwaps = 10) {
    std::vector<int> arr = generateRandom(size);
    std::sort(arr.begin(), arr.end());

    std::uniform_int_distribution<int> indexDist(0, size - 1);
    for (int i = 0; i < numSwaps; i++) {
      int idx1 = indexDist(gen);
      int idx2 = indexDist(gen);
      std::swap(arr[idx1], arr[idx2]);
    }
    return arr;
  }
};

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
static void mergeSort(std::vector<int> &arr, int left, int right) {
  if (left >= right) return;

  int mid = left + (right - left) / 2;
  mergeSort(arr, left, mid);
  mergeSort(arr, mid + 1, right);
  merge(arr, left, mid, right);
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


class SortTester {
 private:
  ArrayGenerator generator;
  int MULTIPLE_RUNS = 5;

 public:
  long long testStandardMergeSort(const std::vector<int> &originalArr) {
    std::vector<int> arr = originalArr;
    auto start = std::chrono::high_resolution_clock::now();
    mergeSort(arr, 0, arr.size() - 1);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
  }

  long long testHybridMergeSort(const std::vector<int> &originalArr, int threshold) {
    std::vector<int> arr = originalArr;
    auto start = std::chrono::high_resolution_clock::now();
    hybridMergeSort(arr, 0, arr.size() - 1, threshold);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
  }

  void runTests() {
    std::ofstream standardFile("/Users/vladbax6/Codding/code_works/cpp/AISDSet1/standard_results.csv");
    std::ofstream hybridFile("/Users/vladbax6/Codding/code_works/cpp/AISDSet1/hybrid_results.csv");

    standardFile << "size,random,reversed,almost_sorted\n";
    hybridFile << "size,threshold,random,reversed,almost_sorted\n";

    std::vector<int> thresholds = {5, 10, 15, 20, 30, 50};

    std::vector<int> baseRandom = generator.generateRandom(100000);
    std::vector<int> baseReversed = generator.generateReversed(100000);
    std::vector<int> baseAlmostSorted = generator.generateAlmostSorted(100000);

    for (int size = 500; size <= 100000; size += 100) {

      std::vector<int> randomArr(baseRandom.begin(), baseRandom.begin() + size);
      std::vector<int> reversedArr(baseReversed.begin(), baseReversed.begin() + size);
      std::vector<int> almostSortedArr(baseAlmostSorted.begin(), baseAlmostSorted.begin() + size);

      long long standardRandom = 0, standardReversed = 0, standardAlmostSorted = 0;

      for (int run = 0; run < MULTIPLE_RUNS; run++) {
        standardRandom += testStandardMergeSort(randomArr);
        standardReversed += testStandardMergeSort(reversedArr);
        standardAlmostSorted += testStandardMergeSort(almostSortedArr);
      }

      standardRandom /= MULTIPLE_RUNS;
      standardReversed /= MULTIPLE_RUNS;
      standardAlmostSorted /= MULTIPLE_RUNS;

      standardFile << size << "," << standardRandom << ","
                   << standardReversed << "," << standardAlmostSorted << "\n";

      for (int threshold : thresholds) {
        long long hybridRandom = 0, hybridReversed = 0, hybridAlmostSorted = 0;

        for (int run = 0; run < MULTIPLE_RUNS; run++) {
          hybridRandom += testHybridMergeSort(randomArr, threshold);
          hybridReversed += testHybridMergeSort(reversedArr, threshold);
          hybridAlmostSorted += testHybridMergeSort(almostSortedArr, threshold);
        }

        hybridRandom /= MULTIPLE_RUNS;
        hybridReversed /= MULTIPLE_RUNS;
        hybridAlmostSorted /= MULTIPLE_RUNS;

        hybridFile << size << "," << threshold << "," << hybridRandom << ","
                   << hybridReversed << "," << hybridAlmostSorted << "\n";
      }

      standardFile.flush();
      hybridFile.flush();
    }

    standardFile.close();
    hybridFile.close();}
};

int main() {
  SortTester tester;
  tester.runTests();
  return 0;
}