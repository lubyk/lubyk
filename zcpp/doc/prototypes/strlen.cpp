#include <iostream>

int main()
{ 
  const char * s = "1234";
  const char * s2 = "";
  std::cout << "strlen(\"" << s << "\") = " << strlen(s) << std::endl;
  std::cout << "strlen(\"" << s2 << "\") = " << strlen(s2) << std::endl;
}