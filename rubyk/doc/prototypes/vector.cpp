#include <iostream>
#include <vector>
#include <list>

#define DO(x) std::cout << #x << " {\n";x;std::cout << "}\n\n";
class Foo
{
 public:
  Foo(const char *name) : name_(name) {
    std::cout << "  " << this << " '" << name_ << "'" << " created.\n";
  }
  
  Foo(const Foo &other) {
    name_ = other.name_;
    std::cout << "  " << this << " '" << name_ << "'" << " copy of " << &other << ".\n";
  }
  
  ~Foo() {
    std::cout << "  " << this << " '" << name_ << "'" << " destroyed.\n";
  }
  std::string name_;
};

int main()
{ 
  std::cout << "======== vector test =======\n";
  DO(std::vector<Foo> vect);
  DO(vect.push_back(Foo("one")));
  DO(vect.insert(vect.begin(), Foo("two")));
  
  std::cout << "======== list test =======\n";
  DO(std::list<Foo> list);
  DO(list.push_back(Foo("one")));
  DO(list.push_front(Foo("two")));
}