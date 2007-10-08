#include <Accelerate/Accelerate.h>
#include <stdlib.h>
#include <stdio.h>

// compute the matrix product A = T'*T
int main () {
  float *T, *A;
  int i;
  // allocate space for the matrices (2x3) and (2x2)
  T  = ( float * ) malloc ( sizeof ( float ) * 2 * 3 );
  A  = ( float * ) malloc ( sizeof ( float ) * 2 * 2 );
  
  T[0+0] =  0; T[0+1] = -1;
  T[2+0] = -1; T[2+1] =  2;
  T[4+0] =  1; T[4+1] = -1;
  
  cblas_sgemm(CblasRowMajor, CblasTrans , CblasNoTrans , 2, 2, 3, 1, T, 2, T, 2, 0.0, A, 2);
  
  for(i=0;i<2;i++) {
    printf("| %.2f %.2f |\n", A[(i*2)+0], A[(i*2)+1]);
  }
  return 0;
}