#include <concepts>
#include <functional>
#include <print>

template <class Key, class Value>
  requires std::equality_comparable<Key>
struct SequentialSearchST {
  struct Node {
    Key key;
    Value val;
    Node *next;
    Node(Key k, Value v, Node *next) : key{k}, val{v}, next{next} {}
  };
  Node *first{nullptr};
  int sz{0};
  SequentialSearchST() {}
  ~SequentialSearchST() {
    while (first) {
      Node *next{first->next};
      delete first;
      first = next;
    }
  }
  int size() { return sz; }
  bool empty() { return size() == 0; }

  bool contains(Key key) { return search(key); }
  Node *search(Key key) {
    for (Node *x = first; x; x = x->next) {
      if (key == x->key)
        return x;
    }
    return nullptr;
  }

  void insert(Key key, Value val) {
    for (Node *x = first; x; x = x->next)
      if (key == x->key) {
        x->val = val;
        return;
      }
    first = new Node(key, val, first);
    ++sz;
  }

  void remove(Key key) { first = remove(first, key); }
  Node *remove(Node *x, Key key) {
    if (x == nullptr)
      return nullptr;
    if (key == x->key) {
      --sz;
      Node *y = x->next;
      delete x;
      return y;
    }
    x->next = remove(x->next, key);
    return x;
  }
};

template <typename T>
concept is_hashable = requires(T a) {
  { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

template <class Key, class Value>
  requires is_hashable<Key>
struct SeparateChainingHashST {
  int N, M;
  std::hash<Key> hashCode;
  SequentialSearchST<Key, Value> *st;

  SeparateChainingHashST()
      : N{0}, M{512}, hashCode{}, st{new SequentialSearchST<Key, Value>[512]} {}

  SeparateChainingHashST(int M)
      : N{0}, M{M}, hashCode{}, st{new SequentialSearchST<Key, Value>[M]} {}

  ~SeparateChainingHashST() { delete[] st; }

  int hash(Key key) const { return (hashCode(key) & 0x7fffffffffffffff) % M; }
  bool contains(Key key) const { return search(key); }
  auto search(Key key) const { return st[hash(key)].search(key); }

  void insert(Key key, Value val) {
    if (!contains(key))
      N++;
    st[hash(key)].insert(key, val);
  }

  void remove(Key key) {
    if (!contains(key))
      N--;
    st[hash(key)].remove(key);
  }

  auto loadFactor() { return N / M; }
  int size() const { return N; }
  bool empty() const { return size() == 0; }
};

int main() {
  SequentialSearchST<int, int> st;
  SeparateChainingHashST<int, int> sc;
  st.insert(0, 0);
  st.insert(1, 1);
  sc.insert(0, 0);
  sc.insert(1, 1);
  if (st.contains(0) && st.contains(1))
    std::print("contained\n");
  if (sc.contains(0) && sc.contains(1))
    std::print("contained\n");
  std::print("{}\n", sizeof(unsigned long));
  st.remove(0);
  st.remove(1);
  sc.remove(0);
  sc.remove(1);
}