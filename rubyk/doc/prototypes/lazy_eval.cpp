#include <iostream>

using namespace std;
bool dec_to_zero(int &i) {
  --i;
  cout << "dec_to_zero: i = " << i << endl;
  return i == 0;
}

int main() {
  int i = 1;
  cout << "false   : i = " << i << endl;
  if (false && dec_to_zero(i)) {
    cout << "(false && dec_to_zero(i)) returned true\n";
  }
  
  cout << "false --: i = " << i << endl;
  if (false && !--i) {
    cout << "(false && !--i) returned true. i = " << i << "\n";
  }
  
  i = 1;
  cout << "true -- : i = " << i << endl;
  if (true && (--i == 0)) {
    cout << "(true && (--i == 0)) returned true. i = " << i << "\n";
  }
  
  i = 2;
  cout << "true -- : i = " << i << endl;
  if (true && (--i == 0)) {
    cout << "(true && (--i == 0)) returned true. i = " << i << "\n";
  }
  cout << "done. i = " << i << endl;
}