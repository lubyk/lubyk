// g++ obs.cpp -I../../../oscit/include -o test
#include <list>
#include <iostream>
#include "oscit/observer.h"
using namespace oscit;


struct Named {
  Named(const char *name) : name_(name) {}
  const char *name_;
};

class B : public Observable, public Named {
public:
  B(const char *name) : Named(name) {}
};

class A : protected Observer, public Observable, public Named {
public:
  A(const char *name) : Named(name), x_(NULL), y_(NULL) {}

  void set_x(Observable *target) {
    set_and_hold(&x_, target);
  }

  void set_y(Observable *target) {
    set_and_hold(&y_, target);
  }

  void *x_ptr() { return &x_; }
  void *y_ptr() { return &y_;}
  void inspect() {
    std::cout << name_ << ": x_ = " << x_ << ", y_ = " << y_ << "\n";
  }
private:
  const char *name_;
  Observable *x_;
  Observable *y_;
};

#define DO(x) std::cout << #x << " {\n";x;std::cout << "}\n\n";

int main() {
  A *a = new A("a");
  B *b = new B("b");
  A *c = new A("c");

  std::cout << "a: " << a << "\n";
  std::cout << "x: " << a->x_ptr() << "\n";
  std::cout << "y: " << a->y_ptr() << "\n\n";
  std::cout << "b: " << b << "\n";
  std::cout << "c: " << c << "\n";

  DO(a->set_x(b));
  DO(a->set_y(b));

  DO(a->set_x(c));
  DO(c->set_x(a));

  DO(delete c);
  a->inspect();
  DO(delete a);
  DO(delete b);
}
