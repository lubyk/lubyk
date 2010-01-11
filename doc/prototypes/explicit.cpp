// g++ creation_order.cpp -o test
#include <iostream>

class Foo {
public:
  Foo(const char *data)
      : data_(data) {
    id_ = ++s_counter_;
    std::cout << "create " << *this << std::endl;
  }

  // cannot declare copy constructor explicit because somehow
  // the call action(Foo("two")) requires an implicit copy constructor
  // (which is finally optimized away)...
  Foo(const Foo &other)
      : data_(other.data_) {
    id_ = ++s_counter_;
    std::cout << "copy   " << other << " in " << *this << std::endl;
  }

private:
  friend std::ostream &operator<<(std::ostream &stream, const Foo &foo);
  static int s_counter_;
  int id_;
  std::string data_;
};

std::ostream &operator<<(std::ostream &stream, const Foo &foo) {
  stream << foo.id_ << ":" << foo.data_;
  return stream;
}

int Foo::s_counter_ = 0;

void action(const Foo &foo) {
  std::cout << "action " << foo << std::endl;
}

int main (int argc, char *argv[]) {
 Foo one("one");
 action(one);
 action(Foo("two"));
 return 0;
}
