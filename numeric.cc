#include <cassert>
#include <cmath>
#include <cstdio>

// Deducing this

auto fib = [](this auto self, int n) {
  if (n == 0)
    return 0;
  if (n == 1)
    return 1;
  return self(n - 1) + self(n - 2);
};

auto gcd = [](this auto self, int p, int q) {
  if (q == 0)
    return p;
  else
    return self(q, p % q);
};

auto factorial = [](this auto self, int n) {
  if (n == 0)
    return 1;
  return n * self(n - 1);
};

// Big Oh

int constant(int n) {
  int count = n;
  count = 1;
  return count;
}

int linear(int n) {
  int count = 0;
  for (int i = 1; i <= n; i *= 2)
    for (int j = 1; j <= i; j++)
      count++;
  return count;
}

int logarithmic(int n) {
  int count = 0;
  for (int i = 1; i < n; i *= 2)
    count++;
  return count;
}

int logfactorial(int n) {
  int count = 0;
  for (int i = 1; i <= n; i++)
    for (int j = 1; j <= i; j *= 2)
      count++;
  return count;
}

int loglinear(int n) {
  int count = 0;
  for (int i = 1; i <= n; i++)
    for (int j = 1; j < n; j *= 2)
      count++;
  return count;
}

int quadratic(int n) {
  int count = 0;
  for (int i = 1; i <= n; i++)
    for (int j = 1; j <= i; j++)
      count++;
  return count;
}

int main() {
  for (int i = 1; i <= 128; i *= 2) {
    assert(constant(i) == 1);
    assert(linear(i) == 2 * i - 1);
    assert(logarithmic(i) == log2(i));
    assert(loglinear(i) == i * log2(i));
    assert(quadratic(i) == (i * i + i) / 2);
  }

  for (int i = 0; i < 8; i++)
    printf("%d\t", fib(i));
  printf("\n");
  for (int i = 0; i < 8; i++)
    printf("%d\t", gcd(i, i + 1));
  printf("\n");
  for (int i = 0; i < 8; i++)
    printf("%d\t", factorial(i));
  printf("\n");
}