#include <cstdio>

template<typename T>
class A
{
public:
  A (const T& value) : mValue(value) {}
  virtual ~A () {}

  void print();

private:
  T mValue;
};

template<typename T>
void A<T>::print()
{
  printf("cannot print\n");
}

template<>
void A<int>::print()
{
  printf("value: %i\n", mValue);
}

int main()
{
  A<double> a(2.3);
  A<int>    b(5);
  a.print();
  b.print(); 
}