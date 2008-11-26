#include <iostream>

using namespace std;

int main()
{  
  cout
       << "bool: "   << 8 * sizeof(bool)           << " bits" << endl
       << "char: "   << 8 * sizeof(char)           << " bits" << endl
       << "int: "    << 8 * sizeof(int)            << " bits" << endl
       << "size_t: " << 8 * sizeof(size_t)         << " bits" << endl
       << "time_t: " << 8 * sizeof(time_t)         << " bits" << endl
       << "long: "   << 8 * sizeof(long)           << " bits" << endl
       << "float: "  << 8 * sizeof(float)          << " bits" << endl
       << "double*: "  << 8*sizeof(double*)        << " bits" << endl
       << "double: " << 8*sizeof(double)           << " bits" << endl
       << "long double: " << 8*sizeof(long double) << " bits" << endl
       ;
}