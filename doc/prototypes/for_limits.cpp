/*
A. without optimization: 7 more instructions for size() vs .mSize.
// compile with "g++ for_limits.cpp -S -DMETH -o test1 && g++ for_limits.cpp -S -o test2 && diff test1 test2" to view assembly code diff

B. with optimization O1: same code.
 compile with "g++ -O1 for_limits.cpp -S -DMETH -o test1 && g++ -O1 for_limits.cpp -S -o test2 && diff test1 test2" to view assembly code diff

*/

#include <cstdio>

class A
{
public:
  A (size_t pSize) : mSize(pSize) {}
  virtual ~A () {}
  
  
  const size_t& size()
  {
    return mSize;
  }
  
  size_t mSize;
};



int main()
{
  A a(10);
  const int j = a.size(); // so the method is not optimized away
  printf("size: %i\n", j);
#ifdef METH
  for(int i=0; i < a.size(); i++) {
    printf("%i\n", i);
  }
#else
  for(int i=0; i < a.mSize; i++) {
    printf("%i\n", i);
  }
#endif
}