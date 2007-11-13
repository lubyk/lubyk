#include <cstdio>

class A
{
public:
  A () {}
  virtual void vswim () = 0;
  
  template< class T, void(T::*Tmethod)(void)>
  inline static void call_swim(A * receiver)
  {
    (((T*)receiver)->*Tmethod)();
  }
};

class B : public A
{
public:
  void vswim();
  
  void swim();
};

class C : public A
{
public:
  void vswim();
  
  void swim();
};

int main()
{
  A * b = new B; // static cast = A, dynamic cast = B
  //void (*caller)(A*) = &A::call_swim<B, &B::swim>;
  
  b->vswim();       // dynamic cast
  //(*caller)(b);     // functor
  ((B*)b)->swim(); // static cast
  
  //(*caller)(b);     // functor
  //b->vswim();       // dynamic cast
  //((B*)b)->swim(); // static cast
}