#include <iostream>
#include <string>

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
       << "bool: "   << 8 * sizeof(bool)           << " bits" << endl
       << "char: "   << 8 * sizeof(char)           << " bits" << endl
       << "value_t: "<< 8 * sizeof(value_t)        << " bits" << endl
       << "int: "    << 8 * sizeof(int)            << " bits" << endl
       << "size_t: " << 8 * sizeof(size_t)         << " bits" << endl
       << "time_t: " << 8 * sizeof(time_t)         << " bits" << endl
       << "long: "   << 8 * sizeof(long)           << " bits" << endl
       << "float: "  << 8 * sizeof(float)          << " bits" << endl
       << "double*: "  << 8*sizeof(double*)        << " bits" << endl
       << "double: " << 8*sizeof(double)           << " bits" << endl
       << "long double: " << 8*sizeof(long double) << " bits" << endl
       << "empty string: " << 8*sizeof(s) << " bits" << endl
       << "foo string: " << 8*sizeof(s2) << " bits" << endl
       ;
}