#include <cassert>
#include <thread>

// 尾指针单链表
template <typename ITEM> class list {
private:
  struct node {
    ITEM item;
    node *next;
    node(ITEM item, node *next) : item{item}, next{next} {}
  };
  node *head{nullptr}, *tail{nullptr};
  int sz{0};

public:
  list() {}
  ~list();

  constexpr bool empty() const { return sz == 0; }
  constexpr int size() const { return sz; }

  void push_back(const ITEM);
  void push_front(const ITEM);
  void pop_back();
  void pop_front();
  void reverse();

  class iterator {
    friend class list;

  public:
    ITEM &operator*() const { return ptr->item; }
    bool operator==(const iterator &rhs) const { return ptr == rhs.ptr; }
    bool operator!=(const iterator &rhs) const { return ptr != rhs.ptr; }
    iterator &operator++();
    iterator operator++(int);

  private:
    iterator(node *ptr) : ptr(ptr) {}
    node *ptr;
  };
  iterator begin() { return iterator(head); }
  iterator end() { return iterator(nullptr); }
};

template <typename ITEM> list<ITEM>::~list() {
  while (head) {
    node *next{head->next};
    delete head;
    head = next;
  }
}

template <typename ITEM> void list<ITEM>::push_back(const ITEM item) {
  node *x{tail};
  tail = new node(item, nullptr);
  if (sz == 0)
    head = tail;
  else
    x->next = tail;
  ++sz;
}

template <typename ITEM> void list<ITEM>::push_front(const ITEM item) {
  head = new node(item, head);
  if (sz == 0)
    tail = head;
  ++sz;
}

template <typename ITEM> void list<ITEM>::pop_back() {
  if (head == tail) {
    delete tail;
    head = tail = nullptr;
  } else {
    node *x{head};
    while (x->next != tail)
      x = x->next;
    delete tail;
    x->next = nullptr;
    tail = x;
  }
  --sz;
}

template <typename ITEM> void list<ITEM>::pop_front() {
  if (head == tail) {
    delete tail;
    head = tail = nullptr;
  } else {
    node *x{head};
    head = x->next;
    delete x;
  }
  --sz;
}

template <typename ITEM> void list<ITEM>::reverse() {
  tail = head;
  node *x{head};
  node *reverse{nullptr};
  while (x) {
    node *y{x};
    x = x->next;
    y->next = reverse;
    reverse = y;
  }
  head = reverse;
}

template <typename ITEM>
typename list<ITEM>::iterator &list<ITEM>::iterator::operator++() {
  ptr = ptr->next;
  return *this;
}

template <typename ITEM>
typename list<ITEM>::iterator list<ITEM>::iterator::operator++(int) {
  iterator x{*this};
  ptr = ptr->next;
  return x;
}

int main() {
  list<int> l;

  std::thread t1(&list<int>::push_front, &l, 1);
  std::thread t2(&list<int>::push_front, &l, 2);
  std::thread t3(&list<int>::push_front, &l, 3);
  t1.join();
  t2.join();
  t3.join();
  assert(l.size() == 3);
  l.pop_back();
  l.pop_back();
  l.pop_back();
  assert(l.size() == 0);

  {
    std::jthread t4(&list<int>::push_back, &l, 4);
    std::jthread t5(&list<int>::push_back, &l, 5);
    std::jthread t6(&list<int>::push_back, &l, 6);
  }
  assert(l.size() == 3);
  l.pop_front();
  l.pop_front();
  l.pop_front();
  assert(l.size() == 0);
}