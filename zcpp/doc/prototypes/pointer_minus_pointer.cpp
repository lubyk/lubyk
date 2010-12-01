#include <iostream>

using namespace std;

int main()
{ 
  const char * buf = "1234567890";
  const char * p = buf;
  
  cout << "buf:     "   << buf << endl;
  cout << "p:       "   << buf << endl;
  cout << "++p:     "   << ++p << endl;
  cout << "++p:     "   << ++p << endl;
  cout << "++p:     "   << ++p << endl;
  cout << "p:       "   << p << endl;
  cout << "p - buf: "    << (p - buf) << endl;
}