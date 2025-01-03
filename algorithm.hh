#include "vector.hh"
#include <cstdlib>
#include <ctime>

namespace ns {
template <class T> struct remove_reference {
  typedef T type;
};
template <class T> struct remove_reference<T &> {
  typedef T type;
};
template <class T> struct remove_reference<T &&> {
  typedef T type;
};
template <class T>
using remove_reference_t = typename remove_reference<T>::type;

// 交换
template <class T> void swap(T &a, T &b) {
  T x{(remove_reference_t<T> &&)a};
  a = (remove_reference_t<T> &&)b;
  b = (remove_reference_t<T> &&)x;
}

// 最值
template <typename T> T max(T a, T b) {
  if (a > b)
    return a;
  else
    return b;
}

// 序检查
template <typename compar> bool is_sorted(const ns::vector<compar> &A) {
  for (int i(1), sz(A.size()); i < sz; ++i)
    if (A[i] < A[i - 1])
      return false;
  return true;
}

// 洗乱
template <typename compar> void shuffle(ns::vector<compar> &A) {
  std::srand(std::time(nullptr));
  for (int i(0), n(A.size()); i < n; ++i) {
    int p{std::rand() % (i + 1)};
    compar x{(remove_reference_t<compar> &&)A[p]};
    A[p] = (remove_reference_t<compar> &&)A[i];
    A[i] = (remove_reference_t<compar> &&)x;
  }
}
} // namespace ns