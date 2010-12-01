// g++ creation_order.cpp -o test
#include <iostream>

struct Foo {
  Foo(const char *name) {
    std::cout << name << " created\n";
  }
};

int main (int argc, char *argv[]) {
 Foo one("start");
 if (argc > 1)
   return 0;

 Foo two("second");
 return 0;
}
