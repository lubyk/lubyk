#include "Matrix.h"

#include <cstdlib>
#include <cstdio>

void Matrix::print_matrix(double* matrix, int pRowCount, int pColCount, int rowMode) {
	int i,j;

  for(i=0;i<pRowCount;i++) {
		printf("|");
		for(j=0;j<pColCount;j++)
		  if (rowMode == CblasRowMajor)
				printf(" %.2f",matrix[(i * pColCount) + j]);
			else
				printf(" %.2f",matrix[(j * pRowCount) + i]);
		printf(" |\n");
  }
}

/** Compute T'T for the given (row major) matrix. Return false on failure. pResult must be a
  * pointer to an array of pCol * pRow doubles. 
  * @param pRowCount number of rows in T.
  * @param pColCount number of columns in T.
  * @param pResult (pColCount * pColCount) buffer for result. */
bool Matrix::compute_symetric_matrix(double ** pResult, const double * pMatrix, int pRowCount, int pColCount)
{
  // you wan to understand this line ?
  // visit: http://dev.gaspardbuma.org/en/post191.html
  cblas_dgemm(CblasRowMajor, CblasTrans , CblasNoTrans , pColCount, pColCount, pRowCount, 1, pMatrix, pColCount, pMatrix, pColCount, 0.0, *pResult, pColCount);
  return true;
}

/** Compute the eigenvectors and eigenvalues for a symmetric (row major) matrix. Return false on failure.
  * @param pRowCount pMatrix square matrix.
  * @param pSize size of square matrix T.
  * @param pEigenVectors pointer to a buffer of size pColCount * pColCount.
  * @param pEigenValues pointer to a buffer of size pColCount. */
bool Matrix::compute_eigenvectors(double ** pEigenVectors, double ** pEigenValues, long * pEigenCount, double * pMatrix, int pSize)
{
  // we have to instanciate all parameters as they are passed by reference.
  char jobz  = 'V';       //  1. 'N': compute eigenvalues only, 'V': eigenvalues and eigenvectors.
  char range = 'A';       //  2. 'A': find all eigenvalues, 
                          //     'V': find all eigenvalues in the interval ]VL,VU]
                          //     'I': find the eigenvalues with index in [IL,IU]
  char uplo  = 'U';       //  3. symmetric matrix is only stored in the upper triangle. 'L': for lower.
  long n   = pSize;  //  4. Order of the matrix A (size of A is NxN).
  // (A) source           //  5.  source array
  long lda = pSize;  //  6. Leading dimension of A (=N).
  double vl = 0.0;         //  7. See RANGE. (L stands for Low)
	double vu = 0.0;         //  8. See RANGE. (U stands for Up)
	long  il = 0.0;         //  9. See RANGE.
	long  iu = 0.0;         // 10. See RANGE.
	double abstol = 0.01;    // 11. Absolute error tolerance for the eigenvalues. If set to zero, 
													//     machine precision will be used during convergence test. Higher values = faster but less precise.
	// (M) eigencount       // 12. Output of the number of eigenvalues found.
  // (W) eigenvalues      // 13. Output eigenvalues in ascending order.
  // (Z) eigenvector      // 14. Output array. First M columns contain the orthonormal eigenvectors. i-th column contains the eigenvector associated with the i-th eigenvalue.
	long ldz = pSize;  // 15. Leading dimension of Z (=N).
 	long isuppz[2 * pSize];// 16. Output array of integers telling which eigenvectors are nonzero. ??
 	double work[pSize * 40];        // 17. Workspace (real array)
  long lwork = pSize * 40;       // 18. Size of the workspace. Should be >= (NB+6)N where NB is the maximal blocksize for SSYTRD and SORMTR returned by ILAENV
 	long iwork[15 * pSize];         // 19. Workspace (integer array)
	long liwork = 15 * pSize;       // 20. Size of IWORK array. Should be >= 10N
	long info = 0;          // 21. Result information. 0 = success, -i: i-th argument had an illegal value, > 0 internal error


  // compute the eigenvectors of pMatrix:
  // you want to understand this line ?
  // visit: http://dev.gaspardbuma.org/en/post191.html
	dsyevr_( &jobz, &range, &uplo, &n, pMatrix, &lda, &vl, &vu, &il, &iu, &abstol, pEigenCount, *pEigenValues, *pEigenVectors, &ldz, isuppz, work, &lwork, iwork, &liwork, &info );
  return true;
}
/*
int main () 
{
double *T, *A;
int i;
long eigencount;
double * eigenvalues = (double*) malloc(2 * sizeof(double));
double * eigenvectors= (double*) malloc(4 * sizeof(double));

// allocate space for the matrices (2x3) and (2x2)
T  = ( double * ) malloc ( sizeof ( double ) * 2 * 3 );
A  = ( double * ) malloc ( sizeof ( double ) * 2 * 2 );

T[0+0] =  0; T[0+1] = -1;
T[2+0] = -1; T[2+1] =  2;
T[4+0] =  1; T[4+1] = -1;


printf("======= Input matrix T    =======\n");
print_matrix(T, 3, 2, CblasRowMajor);

// compute A = T'*T
compute_symetric_matrix(&A,T, 3, 2);
printf("\n======= A = T'*T          =======\n");
print_matrix(A, 2, 2, CblasRowMajor);

compute_eigenvectors(&eigenvectors, &eigenvalues, &eigencount, A, 2);

printf("\n======= eigenvalues of A  =======\n");
print_matrix(eigenvalues, 1, 2, CblasColMajor);

printf("\n======= eigenvectors of A =======\n");
print_matrix(eigenvectors, 2, 2, CblasColMajor);
return 0;
}

*/