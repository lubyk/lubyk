#include <cstdio>
#include <string>
#include <vector>

struct A
{
  A  ()         { printf("Create  A\n"); }
  virtual ~A () { printf("Destroy A\n");  }
  bool init ()  { printf("Init A\n"); }
  void test()   { printf("Test A\n");}
  void call_test() { (*mFunc)(this); }
  void (*mFunc)(A*receiver);
};

struct B : public A
{
  B  ()         { printf("Create  B\n"); }
  virtual ~B () { printf("Destroy B\n");  }
  bool init () { printf("Init B\n"); }
};

struct C : public B
{
  void test()   { printf("Test C\n"); }
};

template<class T>
void cast_test_method(A * ptr)
{
  ((T*)ptr)->test();
}

template<class T>
A * make_class()
{
  T * ptr = new T;
  ptr->init();
  ptr->mFunc = &cast_test_method<T>;
  return (A*)ptr;
}

typedef A * (*create_class_t)();

struct Hash 
{
  template<class T>
  void declare (char * pKey)
  {
    create_class_t ptr = &make_class<T>;
    std::string    key(pKey);
    mFunc.push_back(ptr);
    mKey.push_back(key);
  }
  
  A* new_object(char * pKey)
  {
    int i = 0;
    std::string s(pKey);
    A * tmp;
    while( i < mKey.size() && s != mKey[i] )
      i++;
    if (i < mKey.size()) {
      return (*(mFunc[i]))();
    } else {
      return NULL;
    }
  }
  
  std::vector<create_class_t> mFunc;
  std::vector<std::string>    mKey;
  
};

int main () 
{
  Hash gObjects;
  
  gObjects.declare<A>("metro");
  gObjects.declare<B>("fun");
  gObjects.declare<C>("far");
  A * a;
  A * b;
  A * c;
  
  a = gObjects.new_object("metro");
  b = gObjects.new_object("fun");
  c = gObjects.new_object("far");
  
  printf("\n\n");
  a->call_test(); // Test A
  b->call_test(); // Test A    (B::test() is not defined)
  c->call_test(); // Test C
  
  a->test();      // Test A
  b->test();      // Test A
  c->test();      // Test A    ( test() is not virtual  )
  
  printf("\n\n");
  delete a;
  delete b;
  delete c;
  
  return 0;
}