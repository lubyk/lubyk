// g++ -fexceptions -O0

#include <stdio.h>
//#include <exception>

int main()
{
  //
  try {
    printf("Going to 'throw 1'.\n");
    throw 1;
    printf("Throw did not interrupt control flow.\n");

  } catch(...) {
    printf("'throw 1' caught.\n");
  }

  try {
    printf("Going to divide an integer by zero.\n");
    int r=1;
    int k=0;
    int j=r/k;
    printf("Division by zero did not raise an exception.\n");

  } catch(...) {
    printf("Division by zero caught.\n");
  }

  try {
    printf("Going to divide a float by zero.\n");
    float r=1;
    float k=0;
    float j=r/k;
    printf("Division by zero did not raise an exception.\n");

  } catch(...) {
    printf("Division by zero caught.\n");
  }

  printf("Leaving.\n");

  return 0;
}