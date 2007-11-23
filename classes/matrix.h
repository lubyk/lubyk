#include <Accelerate/Accelerate.h>

class Matrix
{
public:
  /** Print a matrix. */
  static void print_matrix(double* matrix, int pRowCount, int pColCount, int rowMode);
  
  /** Compute T'T for the given (row major) matrix. Return false on failure. pResult must be a
    * pointer to an array of pCol * pRow doubles. 
    * @param pRowCount number of rows in T.
    * @param pColCount number of columns in T.
    * @param pResult (pColCount * pColCount) buffer for result. */
  static bool compute_symetric_matrix(double ** pResult, const double * pMatrix, int pRowCount, int pColCount);


  /** Compute T'T for the given (row major) matrix. Return false on failure. pResult must be a
    * pointer to an array of pCol * pRow doubles. 
    * @param pRowCount pMatrix square matrix.
    * @param pSize size of square matrix T.
    * @param pEigenVectors pointer to a buffer of size pColCount * pColCount.
    * @param pEigenValues pointer to a buffer of size pColCount. \
    * @param pEigenCount result: number of eigenvalues found. */
  static bool compute_eigenvectors(double ** pEigenVectors, double ** pEigenValues, long * pEigenCount, double * pMatrix, int pSize);
};
