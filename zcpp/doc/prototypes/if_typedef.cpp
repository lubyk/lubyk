#include <iostream>

typedef double Real;
using namespace std;

int main()
{ 
#if Real == double
  cout << "Real == double\n";
#else
  cout << "Real != double\n";
#endif
  cout << "done.\n";
}