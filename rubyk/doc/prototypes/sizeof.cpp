#include <iostream>
#include <string>

typedef unsigned int uint;

using namespace std;
enum value_t {
  NilValue  = 0,
  BangValue = 1,
  AnonymousValue = 2,
  NumberValue = 4,
  MatrixValue = 8,
  StringValue = 16,
  CharMatrixValue = 32,
};
int main()
{ 
  string s;
  string s2 = "foo";
  cout
       << "bool: "   << sizeof(bool)           << " bytes" << endl
       << "char: "   << sizeof(char)           << " bytes" << endl
       << "short: "   << sizeof(short)           << " bytes" << endl
       << "uint: "   << sizeof(uint)           << " bytes" << endl
       << "value_t: "<< sizeof(value_t)        << " bytes" << endl
       << "int: "    << sizeof(int)            << " bytes" << endl
       << "size_t: " << sizeof(size_t)         << " bytes" << endl
       << "time_t: " << sizeof(time_t)         << " bytes" << endl
       << "long: "   << sizeof(long)           << " bytes" << endl
       << "float: "  << sizeof(float)          << " bytes" << endl
       << "double*: "  << sizeof(double*)        << " bytes" << endl
       << "double: " << sizeof(double)           << " bytes" << endl
       << "long double: " << sizeof(long double) << " bytes" << endl
       << "empty string: " << sizeof(s) << " bytes" << endl
       << "foo string: " << sizeof(s2) << " bytes" << endl
       ;
}