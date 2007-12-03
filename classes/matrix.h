#include <Accelerate/Accelerate.h>
#include <cstdlib> // fopen, etc.

class CutMatrix;


class Matrix
{
public:
  Matrix() : mRowCount(1), mColCount(0), mErrorBuffer(NULL), mErrorBufferSize(0), mStorageSize(0), data(NULL)
  {
    mErrorMsg = "no error";
  }
  
  Matrix(size_t pRowCount, size_t pColCount) : mRowCount(pRowCount), mColCount(pColCount)
  {
    reallocate(size());
  }
  
  virtual ~Matrix()
  {
    if (mErrorBuffer) free(mErrorBuffer);
    if (data) free(data);
  }
  
  /** Define and allocate the number of rows and columns for the matrix.
    * @return false on allocation failure. */
  bool set_sizes(size_t pRowCount, size_t pColCount)
  {
    if (!reallocate(pRowCount * pColCount)) return false;
    mRowCount = pRowCount;
    mColCount = pColCount;
    return true;
  }
  
  /** Set all values to 'pVal'. */
  void fill(double pVal)
  {
    size_t sz = size();
    for(int i=0; i < sz; i++)
      data[i] = pVal;
  }
  
  /** Set all the elements to 0. */
  void clear ()
  {
    fill(0.0);
  }
  
  /** Return a partial (read-only) version of the matrix.
    * Get the newest 3 vectors:
    * cut(-3, -1)
    *
    * Get the 2 oldest vectors:
    * cut(0, 1)
    */
  const CutMatrix& cut(int pStartRow, int pEndRow);
  
  /** Write a matrix to a FILE pointer. */
  bool to_file(FILE * pFile);
  
  /** Fill a matrix from a FILE pointer.
    * @return false if there was not enough values to fill the matrix. */
  bool from_file(FILE * pFile);
  
  /** Make a partial copy of another matrix (copy all contents).
    * to copy the first vector:
    * copy(matrix, 0, 0)
    * 
    * to copy the third vector:
    * copy(matrix, 2, 2)
    *
    * to copy the last (most recent) 3 lines in the matrix:
    * copy(matrix, -3)
    *
    * @param pOther      other matrix to copy the data from.
    * @param pStartRow   where to start copying the data from (default is 0).
    * @param pEndRow     last row to copy (default is -1 = last row). */
  bool copy(const Matrix& pOther, int pStartRow = 0, int pEndRow = -1)
  {
    return copy_at(0, pOther, pStartRow, pEndRow);
  }
  
  /** Make a partial copy of another matrix starting at a specific row index. The size of the matrix automatically
    * grows to receive the copied data.
    * to copy the second row into the last row, you would write:
    * copy_at(-1, matrix, 1, 1)
    * 
    * to copy the third line into the first row:
    * copy_at(0, matrix, 2, 2)
    *
    * to copy the last (most recent) 3 lines of matrix at the beginning:
    * copy_at(0, matrix, -3)
    *
    * if you need to append at the end of the current matrix, use 'append'.
    *
    * @param pRowIndex   index of the row to start copying data into.
    * @param pOther      other matrix to copy the data from.
    * @param pStartRow   where to start copying the data from (default is 0).
    * @param pEndRow     last row to copy (default is -1 = last row).
    *
    * @return bool       returns false if allocation of new space failed. */
  bool copy_at(int pRowIndex, const Matrix& pOther, int pStartRow = 0, int pEndRow = -1);

  
  /** Append a vector to the end of the current data. Size increases automatically. */
  bool append (const double * pVector, size_t pVectorSize);
  
  /** Append another matrix/vector to the end of the current data. Size increases automatically. 
    * @param pStartRow   where to start copying the data from (default is 0).
    * @param pEndRow     last row to copy (default is -1 = last row).
    * @return false if the column count of both matrices do not match. */
  bool append (const Matrix& pOther, int pStartRow = 0, int pEndRow = -1);
  
  
  
  /** Add elements of one matrix to another.
    * If rows/columns match, elements are added one by one.
    * If the other matrix is a vector and columns sizes match, the vector is added to each row.
    * If the other matrix is a column vector and row counts match, the scalar is used against the corresponding row.
    * If the other matrix is a scalar, add the value to all elements.
    *
    * @param pOther other matrix whose elements will be added.
    * @param pStartRow if you want to use only part of the other matrix, start row. Default 0 (first row).
    * @param pEndRow   when using only part of the other matrix. Default -1 (last row).
    * @param pScale    amount to multiply each value before adding.
    *
    * @return true (never fails). */
  bool add (const Matrix& pOther, int pStartRow = 0, int pEndRow = -1, double pScale = 1.0);
  
  /** Divide all elements by the values in another matrix. a.divide(b) (a/b) is NOT the matrix division (a-1b)
    * If rows/columns match, elements are divided one by one.
    * If the other matrix is a vector and columns sizes match, each row is divided by the vector.
    * If the other matrix is a column vector and row counts match, corresponding rows are divided by the scalar.
    * If the other matrix is a scalar, divide all element by this value.
    *
    * @param pOther other matrix by which the elements of this matrix will be divided.
    * @param pStartRow if you want to use only part of the other matrix, start row. Default 0 (first row).
    * @param pEndRow   when using only part of the other matrix. Default -1 (last row).
    * @return true (never fails). */
  bool divide (const Matrix& pOther, int pStartRow = 0, int pEndRow = -1, double pScale = 1.0);
  
  /** Divide all elements by the values in another matrix.
    * See 'divide' for details. */
  bool operator/= (const Matrix& pOther)
  {
    return divide(pOther);
  }
  
  /** Divide all elements by a scalar.
    *
    * @param pValue  value by which to divide all elements of the current matrix.
    *
    * @return true (never fails). */
  bool operator/= (double pValue)
  {
    size_t sz = size();
    for(int i=0; i < sz; i++)
      data[i] /= pValue;
    return true;
  }
  
  /** Multiply all elements by the values in another matrix. a.divide(b) (a/b) is NOT the matrix multiplication (ab). See 'mat_multiply'.
    * If rows/columns match, elements are multiplied one by one.
    * If the other matrix is a vector and columns sizes match, each row is multiplied by the vector.
    * If the other matrix is a column vector and row counts match, corresponding rows are multiplied by the scalar.
    * If the other matrix is a scalar, multiply all element by this value.
    *
    * @param pOther other matrix by which the elements of this matrix will be divided.
    * @param pStartRow if you want to use only part of the other matrix, start row. Default 0 (first row).
    * @param pEndRow   when using only part of the other matrix. Default -1 (last row).
    * @return true (never fails). */
  bool multiply (const Matrix& pOther, int pStartRow = 0, int pEndRow = -1, double pScale = 1.0);
  
  /** Divide all elements by the values in another matrix.
    * See 'divide' for details. */
  bool operator*= (const Matrix& pOther)
  {
    return multiply(pOther);
  }
  
  /** Divide all elements by a scalar.
    *
    * @param pValue  value by which to divide all elements of the current matrix.
    *
    * @return true (never fails). */
  bool operator*= (double pValue)
  {
    size_t sz = size();
    for(int i=0; i < sz; i++)
      data[i] *= pValue;
    return true;
  }
  
  /** Substract all elements by the values in another matrix.
    * See 'add' for details. */
  bool operator-= (const Matrix& pOther)
  {
    return add(pOther, 0, -1, -1.0);
  }
  
  /** Substract a value to all elements in the matrix. */
  bool operator-= (double pValue)
  {
    size_t sz = size();
    for(int i=0; i < sz; i++)
      data[i] -= pValue;
    return true;
  }
  
  /** Add elements of another matrix. See 'add' for details. */
  bool operator+= (const Matrix& pOther)
  {
    return add(pOther);
  }
  
  /** Add a value to all elements in the matrix. */
  bool operator+= (double pValue)
  {
    size_t sz = size();
    for(int i=0; i < sz; i++)
      data[i] += pValue;
    return true;
  }
  
  /** Return the message for the last error. */
  const char * error_msg()
  {
    return mErrorMsg;
  }
  
  /** Print the matrix (usefull for debugging). Use 'to_file' to serialize. */
  void print(FILE * file = stdout);
  
  ////////////////  MATRIX OPERATIONS ////////////////////////
  /** Matrix multiplication.
    * Write C.mat_multiply(A, B) for C = AB
    *
    * @param A matrix A.
    * @param B matrix B.
    * @param pTransA transposition mode for matrix A (CblasNoTrans/CblasTrans).
    * @param pTransB transposition mode for matrix B.
    * @param pScale  scale factor. Default is 1.0 (no scaling). */
  bool mat_multiply(const Matrix& A, const Matrix& B, const enum CBLAS_TRANSPOSE pTransA = CblasNoTrans, const enum CBLAS_TRANSPOSE pTransB = CblasNoTrans, double pScale = 1.0);
  
  ////////////////  static methods  //////////////////////////
  
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
    * @param pEigenVectors pointer to a buffer of size pColCount * pColCount to store resulting eigenvectors. First eigenvector (corresponding to smallest eigenvalue) is in the first column
    * @param pEigenValues pointer to a buffer of size pColCount to store resulting eigenvalues (in ascending order).
    * @param pEigenCount result: number of eigenvalues found. */
  static bool compute_eigenvectors(double ** pEigenVectors, double ** pEigenValues, long * pEigenCount, double * pMatrix, int pSize);
  
  /** Return a pointer to the first element in the row pointed to by 'pIndex'. 
    * You have to make sure pIndex is smaller the mRowCount. Ne verification is done here. */
  double * operator[] (size_t pIndex)
  {
    return data + mColCount * pIndex;
  }
  
  /** Return a pointer to the first element in the row pointed to by 'pIndex'. 
    * You have to make sure pIndex is smaller the mRowCount. Ne verification is done here. */
  const double * operator[] (size_t pIndex) const
  {
    return data + mColCount * pIndex;
  }
  
  /** Return the number of columns in the matrix. */
  const size_t& row_count() const
  {
    return mRowCount;
  }
  
  /** Return the number of columns in the matrix. */
  const size_t& col_count() const
  {
    return mColCount;
  }
  
  size_t size() const
  {
    return mRowCount * mColCount;
  }
  
public:
  double * data; /**< Pointer to the data. Public for performance, handle with care. */
  
protected:
  friend class CutMatrix;
  /** Manage memory allocation (make sure there is enough space for matrix of size pSize). 
    * @return false on memory allocation failure. */
  bool check_alloc(size_t pSize);
  
  /** Verify sizes before operations. */
  bool check_sizes(const char * pMsg, size_t * start_row, size_t * end_row, const Matrix& pOther, int pStartRow, int pEndRow, bool pAllowColCountChange = false);

  /** The size of the matrix changed. We need to increase/decrease memory usage.
    * @return false on memory allocation failure. */
  bool reallocate(size_t pSize);  
  
  /** Copy data using memcpy. (Update size if needed).
    * @param pRowOffset where to start copying (set to mRowCount to append at end). */
  bool raw_copy(size_t pRowOffset, const double * pVector, size_t pVectorSize);

  bool set_error(const char * fmt, ...);
  
  const char * mErrorMsg; /**< Pointer to the last error message. */
  char * mErrorBuffer; /**< Can be used to print custom messages. */
  size_t mErrorBufferSize;
  size_t mStorageSize; /**< Storage size is greater or equal to mRowCount * mColCount. */
  size_t    mRowCount; /**< Number of rows in the matrix. */
  size_t    mColCount; /**< Number of columns in the matrix. mRowCount x mColCount can be greater then mSize during partial append. */

};


/** Read-only matrix showing part of another matrix. */
class CutMatrix : public Matrix
{
public:
  CutMatrix(Matrix& pOther, size_t pStartRow, size_t pEndRow)
  {
    size_t end_row = pEndRow < 0 ? pOther.mRowCount + pEndRow : pEndRow;
    if (pStartRow >= pOther.mRowCount || pStartRow < 0) {
      data = NULL;
      mColCount = 0;
      mRowCount = 0;
      return;
    } else if (end_row >= pOther.mRowCount) {
      data = NULL;
      mColCount = 0;
      mRowCount = 0;
      return;
    }
    data = pOther.data + pStartRow * pOther.mColCount;
    mRowCount = end_row - pStartRow + 1;
    mColCount = pOther.mColCount;
  }
  
  CutMatrix(double * pData, size_t pRowCount, size_t pColCount) {
    data = pData;
    mRowCount = pRowCount;
    mColCount = pColCount;
  }
  
  virtual ~CutMatrix()
  {
    data = NULL; // make sure it is never freed.
  }
  
  void set_sizes(size_t pRowCount, size_t pColCount)
  {
    mRowCount = pRowCount;
    mColCount = pColCount;
  }
  
  void set_data(double * pData)
  {
    data = pData;
  }
};
