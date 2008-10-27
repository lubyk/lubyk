#include <cstdio>
#define FUN(klass,method) (Functor::make_functor<klass, &klass::method>(#method))

class Functor
{
public:
  template<class T, void (T::*Tmethod)(void)>
  static Functor& make_functor (const char * pName)
  {
    Functor * f = new Functor();
    f->mFunc = &cast<T,Tmethod>;
    f->mMethodName = pName;
    return *f;
  }
  
  void operator() (void * pReceiver)
  {
    printf("Calling '%s'\n", mMethodName);
    (*mFunc)(pReceiver);
  }
private:
  template<class T, void (T::*Tmethod)(void)>
  static void cast(void * pReceiver)
  {
    (((T*)pReceiver)->*Tmethod)();
  }
  void (*mFunc)(void*);
  const char * mMethodName;
};

class A
{
public:
  A(const char * name) : mName(name) {}
  
  void hello()
  {
    printf("Hello from A(%s)\n", mName);
  }
private:
  const char * mName;
};

int main()
{
  A a("Gaspard");
  Functor f = FUN(A, hello);
  f(&a);
}