#include "vector.hh"
#include <algorithm>
#include <concepts>
#include <print>
#include <random>

template <typename compar>
  requires std::totally_ordered<compar>
struct QuickSelect {
  static compar select(ns::vector<compar> a, int key) {
    std::ranges::shuffle(a, std::mt19937(std::random_device{}()));
    int lo = 0, hi = a.size() - 1;
    while (lo < hi) {
      int pivot = partition(a, lo, hi);
      if (pivot == key)
        return a[key];
      else if (pivot > key)
        hi = pivot - 1;
      else if (pivot < key)
        lo = pivot + 1;
    }
    return a[key];
  }
  static int partition(ns::vector<compar> &A, int p, int r) {
    compar x = A[r];
    int i = p - 1;
    for (int j = p; j < r; ++j) {
      if (A[j] <= x) {
        ++i;
        std::swap(A[i], A[j]);
      }
    }
    std::swap(A[i + 1], A[r]);
    return i + 1;
  }
};

int main() {
  std::mt19937 mt(std::random_device{}());
  std::uniform_int_distribution rand(10, 99);

  ns::vector<int> a(12);
  for (int i = 0; i < 12; ++i)
    std::print("{}\t", i);
  std::print("\n\n");

  for (auto &e : a) {
    e = rand(mt);
    std::print("{}\t", e);
  }
  std::print("\n\n");

  for (int i = 0; i < 12; ++i)
    std::print("{}\t", QuickSelect<int>::select(a, i));
  std::print("\n");
}