#include <stdio.h>
#include <string>

struct Foo {};

struct A {
  std::string name_;
  A(const char *name)
    : name_(name) {}
};

struct B {
  float age_;
  B(float age)
    : age_(age) {}
};

struct C : public A, public B {
  C(const char *name, float age)
    : A(name)
    , B(age) {}
};

int main() {
  C *c = new C("Hello", 3.21);

  /// This is what we get in the binding: a void* pointer.
  void *ptr = (void*)c;

  A *a = reinterpret_cast<A*>(ptr);
  B *b = reinterpret_cast<B*>(ptr);
  printf("reinterpret_cast\n");
  printf("c: [%p] %s, %f\n", c, c->name_.c_str(), c->age_);
  printf("a: [%p] %s\n", a, a->name_.c_str());
  printf("b: [%p] %f\n", b, b->age_);
  // Just to have a pointer to class (and then we let RTTI decide)
  Foo *f = (Foo*)ptr;
  *a = dynamic_cast<A*>(f);
  *b = dynamic_cast<B*>(f);
  printf("dynamic_cast\n");
  printf("c: [%p] %s, %f\n", c, c->name_.c_str(), c->age_);
  printf("a: [%p] %s\n", a, a->name_.c_str());
  printf("b: [%p] %f\n", b, b->age_);
  return 0;
}
