#include <stdio.h>

class A {
  float data;
public:
  A() : data(0) {}

  void test() {
    data = data + 1;
    printf("A: %p (%f)\n", this, data);
  }
};

class B {
  float data_b;
public:
  B() : data_b(0) {}
  void test() {
    data_b = data_b + 10;
    printf("B: %p (%f)\n", this, data_b);
  }
};

class C : public A {
  float foo;
public:
  C() : foo(0) {}
  void test() {
    printf("C: %p (%f)\n", this, foo);
    A::test();
  }
};

struct D {
  float a;
  float c;
};

int main() {
  C *c = new C();
  D *d = (D*)c;

  d->a = 10;
  d->c = 3;

  c->test();
  return 0;
}
