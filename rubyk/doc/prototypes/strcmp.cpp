// build with
// g++ strcmp.cpp -o test
///////////// MACRO TO GET STATIC STRING SIZE ////////////////
#define STRSZ(string) (*string == 0 ? 0 : 1 + STRSZ_RECURSE_01(string+1))
#define STRSZ_RECURSE_01(string) (*(string) == 0 ? 0 : 1 + STRSZ_RECURSE_02(string+1))
#define STRSZ_RECURSE_02(string) (*(string) == 0 ? 0 : 1 + STRSZ_RECURSE_03(string+1))
#define STRSZ_RECURSE_03(string) (*(string) == 0 ? 0 : 1 + STRSZ_RECURSE_04(string+1))
#define STRSZ_RECURSE_04(string) (*(string) == 0 ? 0 : 1 + STRSZ_RECURSE_05(string+1))
#define STRSZ_RECURSE_05(string) (*(string) == 0 ? 0 : 1 + STRSZ_RECURSE_06(string+1))
#define STRSZ_RECURSE_06(string) (*(string) == 0 ? 0 : 1 + STRSZ_RECURSE_07(string+1))
#define STRSZ_RECURSE_07(string) (*(string) == 0 ? 0 : 1 + STRSZ_RECURSE_08(string+1))
#define STRSZ_RECURSE_08(string) (*(string) == 0 ? 0 : 1 + STRSZ_RECURSE_09(string+1))
#define STRSZ_RECURSE_09(string) (*(string) == 0 ? 0 : 1 + STRSZ_RECURSE_10(string+1))
#define STRSZ_RECURSE_10(string) (*(string) == 0 ? 0 : 1 + STRSZ_RECURSE_11(string+1))
#define STRSZ_RECURSE_11(string) (*(string) == 0 ? 0 : 1 + STRSZ_RECURSE_12(string+1))
#define STRSZ_RECURSE_12(string) (*(string) == 0 ? 0 : 1 + STRSZ_RECURSE_13(string+1))
#define STRSZ_RECURSE_13(string) (*(string) == 0 ? 0 : 1 + STRSZ_RECURSE_14(string+1))
#define STRSZ_RECURSE_14(string) (*(string) == 0 ? 0 : 1 + STRSZ_RECURSE_15(string+1))
#define STRSZ_RECURSE_15(string) (*(string) == 0 ? 0 : 1 + STRSZ_RECURSE_16(string+1))
#define STRSZ_RECURSE_16(string) (*(string) == 0 ? 0 : 1 + STRSZ_RECURSE_17(string+1))
#define STRSZ_RECURSE_17(string) (*(string) == 0 ? 0 : 1 + STRSZ_RECURSE_18(string+1))
#define STRSZ_RECURSE_18(string) (*(string) == 0 ? 0 : 1 + STRSZ_RECURSE_19(string+1))
#define STRSZ_RECURSE_19(string) (*(string) == 0 ? 0 : 1 + STRSZ_RECURSE_20(string+1))
#define STRSZ_RECURSE_20(string) (0)

#include <iostream>
#include <string.h>

void compare1(const char *a, const char *b) {
  std::cout << " strcmp(\"" << a << "\",\"" << b << "\") = " << strcmp(a, b) << std::endl;
}


void compare2(const char *a, const char *b, size_t num) {
  std::cout << "strncmp(\"" << a << "\",\"" << b << "\", " << num << ") = " << strncmp(a, b, num) << std::endl;
}

int main () {
  compare1("Object", "Object.Root");
  compare2("Object", "Object.Root", STRSZ("Object"));
  compare1("Object.Root", "Object");
  compare2("Object.Root", "Object", STRSZ("Object.Root"));
  compare1("Object", "Object");
  compare2("Object", "Object", STRSZ("Object"));
  compare1("XY", "XAB");
  compare2("XY", "XAB", STRSZ("XY"));
  return 0;
}
