#include <Accelerate/Accelerate.h>
#include <stdlib.h>
#include <stdio.h>

/* compute the eigenvalues of the matrix product A = T'*T
   T'*T = | 2  -3 |
          | -3  6 |

   eigen values : 0.4, 7.6
   
   eigen vectors : | 0.9   0.5 |
                   | 0.5  -0.9 |
*/

// compute the eigenvectors and eigenvalues for a symmetric matrix 'source'.
static int sym_eigen(float* source, long sourceSize, long* eigencount, float* eigenvalues, float* eigenvectors) {
  // we have to instanciate all parameters as they are passed by reference.
  char jobz  = 'V';       //  1. 'N': compute eigenvalues only, 'V': eigenvalues and eigenvectors.
  char range = 'A';       //  2. 'A': find all eigenvalues, 
                          //     'V': find all eigenvalues in the interval ]VL,VU]
                          //     'I': find the eigenvalues with index in [IL,IU]
  char uplo  = 'U';       //  3. symmetric matrix is only stored in the upper triangle. 'L': for lower.
  long n   = sourceSize;  //  4. Order of the matrix A (size of A is NxN).
  // (A) source           //  5.  source array
  long lda = sourceSize;  //  6. Leading dimension of A (=N).
  float vl = 0.0;         //  7. See RANGE. (L stands for Low)
	float vu = 0.0;         //  8. See RANGE. (U stands for Up)
	long  il = 0.0;         //  9. See RANGE.
	long  iu = 0.0;         // 10. See RANGE.
	float abstol = 0.01;    // 11. Absolute error tolerance for the eigenvalues. If set to zero, 
													//     machine precision will be used during convergence test. Higher values = faster but less precise.
	// (M) eigencount       // 12. Output of the number of eigenvalues found.
  // (W) eigenvalues      // 13. Output eigenvalues in ascending order.
  // (Z) eigenvector      // 14. Output array. First M columns contain the orthonormal eigenvectors. i-th column contains the eigenvector associated with the i-th eigenvalue.
	long ldz = sourceSize;  // 15. Leading dimension of Z (=N).
 	long isuppz[2 * sourceSize];// 16. Output array of integers telling which eigenvectors are nonzero. ??
 	float work[200];        // 17. Workspace (real array)
  long lwork = 200;       // 18. Size of the workspace. Should be >= (NB+6)N where NB is the maximal blocksize for SSYTRD and SORMTR returned by ILAENV
 	long iwork[20];         // 19. Workspace (integer array)
	long liwork = 20;       // 20. Size of IWORK array. Should be >= 10N
	long info = 0;          // 21. Result information. 0 = success, -i: i-th argument had an illegal value, > 0 internal error
	
	
  // compute the eigenvectors of A:
	ssyevr_( &jobz, &range, &uplo, &n, source, &lda, &vl, &vu, &il, &iu, &abstol, eigencount, eigenvalues, eigenvectors, &ldz, isuppz, work, &lwork, iwork, &liwork, &info );
}

void print_matrix(float* matrix, int m, int n, int rowMode) {
	int i,j;
	
  for(i=0;i<m;i++) {
		printf("|");
		for(j=0;j<n;j++)
		  if (rowMode == CblasRowMajor)
				printf(" %.2f",matrix[(i * n) + j]);
			else
				printf(" %.2f",matrix[(j * m) + i]);
		printf(" |\n");
  }
}
  
int main () {
  float *T, *A;
  int i;
	long eigencount;
	float eigenvalues[2];
	float eigenvectors[4];
	
  // allocate space for the matrices (2x3) and (2x2)
  T  = ( float * ) malloc ( sizeof ( float ) * 2 * 3 );
  A  = ( float * ) malloc ( sizeof ( float ) * 2 * 2 );
  
  T[0+0] =  0; T[0+1] = -1;
  T[2+0] = -1; T[2+1] =  2;
  T[4+0] =  1; T[4+1] = -1;
  
  
	printf("======= Input matrix T    =======\n");
	print_matrix(T, 3, 2, CblasRowMajor);
	
  // compute A = T'*T
  cblas_sgemm(CblasRowMajor, CblasTrans , CblasNoTrans , 2, 2, 3, 1, T, 2, T, 2, 0.0, A, 2);
	printf("\n======= A = T'*T          =======\n");
	print_matrix(A, 2, 2, CblasRowMajor);
	
	sym_eigen(A, 2, &eigencount, eigenvalues, eigenvectors);
	
	printf("\n======= eigenvalues of A  =======\n");
	print_matrix(eigenvalues, 1, 2, CblasColMajor);
	
	printf("\n======= eigenvectors of A =======\n");
	print_matrix(eigenvectors, 2, 2, CblasColMajor);
  return 0;
}