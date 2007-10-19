#include <cstdio>
#include <string>
#include <vector>

struct A
{
  A  ()         { printf("Creating A\n"); }
  virtual ~A () { printf("Destroy A\n");  }
  void kill ()
  {
    (*destroy)((void*)this);
  }
  void (*destroy)(void * ptr);
};

struct B : public A
{
  B  ()         { printf("Creating B\n"); }
  virtual ~B () { printf("Destroy B\n");  }
};


template<class T>
void delete_class(void * ptr)
{
  delete (T*)ptr;
}

template<class T>
void * make_class()
{
  T * ptr = new T;
  ptr->destroy = &delete_class<T>;
  return (void *)ptr;
}

typedef void * (*create_class_t)();

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
    while( i < mKey.size() && s != mKey[i] )
      i++;
    if (i < mKey.size()) {
      return (A*)(*(mFunc[i]))();
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
  A * a;
  A * b;
  
  a = gObjects.new_object("metro");
  b = gObjects.new_object("fun");
  
  a->kill();
  b->kill();
  
  return 0;
}