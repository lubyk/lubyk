/*
A. without optimization: same size (but a LOT more then with O3)
// compile with "g++ data_accessor.cpp -S -DMETH -o test1 && g++ data_accessor.cpp -S -o test2 && diff test1 test2" to view assembly code diff

B. with optimization O3: 5 more instructions with operator[] then .mData[]
 compile with "g++ -O3 data_accessor.cpp -S -DMETH -o test1 && g++ -O3 data_accessor.cpp -S -o test2 && diff test1 test2" to view assembly code diff

*/

#include <cstdio>

class A
{
public:
  A () {}
  virtual ~A () {}
  
  int& operator[] (int pIndex)
  {
    return mData[pIndex];
  }

  int mData[10];
};



int main()
{
  A a;
  a[0] = 0; // make sure it is not optimized away
#ifdef METH
  for(int i=0; i < 10; i++) {
    a[i] = i;
  }
#else
  for(int i=0; i < 10; i++) {
    a.mData[i] = i;
  }
#endif
}