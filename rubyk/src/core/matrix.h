#ifndef _MATRIX_H_
#define _MATRIX_H_
#include "rubyk_types.h"
#include <Accelerate/Accelerate.h>
#include <cstdlib> // fopen, etc.

#define MATRIX_MIN_DET 1e-10 // minimal determinant to compute matrix inversion

class CutMatrix;
union Value;


// taken from Quake
inline float fast_sqrt(const float val) {
  float x = val;
  float xhalf = 0.5f * x;
  int i = *(int*)&x; // store floating-point bits in integer
  i = 0x5f3759d5 - (i >> 1); // initial guess for Newton's method
  x = *(float*)&i; // convert new bits into float
  x = x*(1.5f - xhalf*x*x); // One round of Newton's method
  return val * x;  // x = 1/sqrt(val)
}

template<typename T>
class TMatrix
{
public:
  TMatrix() :  data(NULL), mStorageSize(0), mRowCount(0), mColCount(0), mPlaneCount(1), mErrorBuffer(NULL), mErrorBufferSize(0)
  {
    mErrorMsg = "no error";
  }
  
  TMatrix(size_t pRowCount, size_t pColCount) :  data(NULL), mStorageSize(0), mRowCount(pRowCount), mColCount(pColCount), mErrorBuffer(NULL), mErrorBufferSize(0)
  {
    mErrorMsg = "no error";
    reallocate(size());
  }
  
  virtual ~TMatrix()
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
  void fill(T pVal)
  {
    size_t sz = size();
    for(size_t i=0; i < sz; i++)
      data[i] = pVal;
  }
  
  /** Set all the elements to 0. */
  void clear ()
  {
    fill(0.0);
  }
  
  /** Write a matrix to a FILE pointer. 
      @param iMatrix if true, will write to return after write. Set to 
             false to record a partial matrix.
    */
  bool to_file(FILE * pFile, bool isMatrix = true) const;
  
  /** Write a matrix to a filepath. */
  bool to_file(const std::string& pPath, const char * pMode = "wb", bool isMatrix = true) const;
  
  /** Fill a matrix from a FILE pointer.
    * If the matrix has '0' rows, all the file is loaded into the matrix. If
    * the matrix has '0' columns, the size is built from the number of values until
    * the next newline.
    *
    * @return false if there was not enough values to fill the matrix. */
  bool from_file(FILE * pFile);
  
  /** Read a matrix from a filepath. */
  bool from_file(const std::string& pPath, const char * pMode = "rb");
  
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
  bool copy(const TMatrix& pOther, int pStartRow = 0, int pEndRow = -1)
  {
    return copy_at(0, pOther, pStartRow, pEndRow, true);
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
    * @param pResize     if true and pRowIndex is '0', the matrix is resized to the copied data.
    *
    * @return bool       returns false if allocation of new space failed. */
  bool copy_at(int pRowIndex, const TMatrix& pOther, int pStartRow = 0, int pEndRow = -1, bool pResize = false);

  bool copy(const Value& sig);
  
  bool copy_at(int pRowIndex, const Value& sig);
  
  /** Append a vector to the end of the current data. Size increases automatically. */
  bool append (const T * pVector, size_t pVectorSize);
  
  /** Cast & append a vector. Size increases automatically. */
  template<typename V>
  bool cast_append (const V * pVector, size_t pVectorSize, real_t pScale = 1.0);
  
  /** Append another matrix/vector to the end of the current data. Size increases automatically. 
    * @param pStartRow   where to start copying the data from (default is 0).
    * @param pEndRow     last row to copy (default is -1 = last row).
    * @return false if the column count of both matrices do not match. */
  bool append (const TMatrix& pOther, int pStartRow = 0, int pEndRow = -1);
  
  
  /** Append a value at the end of a vector. Size increases automatically. */
  bool append (T pValue);
  
  /** Add elements of one matrix to another.
    * If rows/columns match, elements are added one by one.
    * If the other matrix is a vector and column sizes match, the vector is added to each row.
    * If the other matrix is a column vector and row counts match, the scalar is used against the corresponding row.
    * If the other matrix is a scalar, add the value to all elements.
    *
    * @param pOther other matrix whose elements will be added.
    * @param pStartRow if you want to use only part of the other matrix, start row. Default 0 (first row).
    * @param pEndRow   when using only part of the other matrix. Default -1 (last row).
    * @param pScale    amount to multiply each value before adding.
    *
    * @return true (never fails). */
  bool add (const TMatrix& pOther, int pStartRow = 0, int pEndRow = -1, real_t pScale = 1.0);
  
  /** Subtract elements of one matrix to another.
    * See 'add' for details. */
  inline bool subtract(const TMatrix& pOther, int pStartRow = 0, int pEndRow = -1, real_t pScale = 1.0)
  { return add(pOther, pStartRow, pEndRow, -pScale); }
  
  /** Add an array of reals to each elements in the matrix. 
    * If the size is the same as the matrix : one to one.
    * If the size is col_size : add to each row.
    * If the size is row_size : add corresponding value to element in the row. */
  bool add (const T * pVector, size_t pVectorSize);
  
  /** Add two matrices. */
  bool add (const TMatrix& A, const TMatrix& B, real_t pScaleA = 1.0, real_t pScaleB = 1.0);
  
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
  bool divide (const TMatrix& pOther, int pStartRow = 0, int pEndRow = -1, real_t pScale = 1.0);
  
  /** Divide all elements by the values in another matrix.
    * See 'divide' for details. */
  bool operator/= (const TMatrix& pOther)
  {
    return divide(pOther);
  }
  
  /** Divide all elements by a scalar.
    *
    * @param pValue  value by which to divide all elements of the current matrix.
    *
    * @return true (never fails). */
  bool operator/= (T pValue)
  {
    size_t sz = size();
    for(size_t i=0; i < sz; i++)
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
  bool multiply (const TMatrix& pOther, int pStartRow = 0, int pEndRow = -1, real_t pScale = 1.0);
  
  /** Divide all elements by the values in another matrix.
    * See 'divide' for details. */
  bool operator*= (const TMatrix& pOther)
  {
    return multiply(pOther);
  }
  
  /** Divide all elements by a scalar.
    *
    * @param pValue  value by which to divide all elements of the current matrix.
    *
    * @return true (never fails). */
  bool operator*= (T pValue)
  {
    size_t sz = size();
    for(size_t i=0; i < sz; i++)
      data[i] *= pValue;
    return true;
  }
  
  /** Substract all elements by the values in another matrix.
    * See 'add' for details. */
  bool operator-= (const TMatrix& pOther)
  {
    return add(pOther, 0, -1, -1.0);
  }
  
  /** Substract a value to all elements in the matrix. */
  bool operator-= (T pValue)
  {
    size_t sz = size();
    for(size_t i=0; i < sz; i++)
      data[i] -= pValue;
    return true;
  }
  
  /** Add elements of another matrix. See 'add' for details. */
  bool operator+= (const TMatrix& pOther)
  {
    return add(pOther);
  }
  
  /** Add a value to all elements in the matrix. */
  bool operator+= (T pValue)
  {
    size_t sz = size();
    for(size_t i=0; i < sz; i++)
      data[i] += pValue;
    return true;
  }
  
  /** Return the message for the last error. */
  const char * error_msg() const
  {
    return mErrorMsg;
  }
  
  /** Print the matrix (usefull for debugging). Use 'to_file' to serialize. */
  void print(FILE * pFile = stdout) const;
  
  ////////////////  MATRIX OPERATIONS ////////////////////////
  /** Matrix multiplication.
    * Write C.mat_multiply(A, B) for C = AB
    *
    * @param A matrix A.
    * @param B matrix B.
    * @param pTransA transposition mode for matrix A (CblasNoTrans/CblasTrans).
    * @param pTransB transposition mode for matrix B.
    * @param pScale  scale factor. Default is 1.0 (no scaling). */
  bool mat_multiply(const TMatrix& A, const TMatrix& B, const enum CBLAS_TRANSPOSE pTransA = CblasNoTrans, const enum CBLAS_TRANSPOSE pTransB = CblasNoTrans, real_t pScale = 1.0);
  
  
  /** Compute A'A for the given (row major) matrix. Return false on failure. Write C.symmetric(A) for C = A'A. */
  bool symmetric(const TMatrix& A);

  /** Set the matrix to the eigenvectors of a symmetric (row major) matrix. Return false on failure.
    * @param pEigenValues will contain the eigenvalues (in ascending order).
    * @param pMatrix source symmetric matrix (will be altered during processing. Send a copy if you want it kept clean).
    */
  bool eigenvectors(TMatrix& pEigenValues, TMatrix& pMatrix);
  
  
  /** Compute in-place inverse of a square matrix.
    * @eturn false on failure.
    */
  bool inverse();
  
  /** Create an identity matrix of size 'n'.
    * @param n size of identity matrix. */
  bool identity(size_t pRowCount)
  {
    if (!set_sizes(pRowCount, pRowCount)) return false;
    clear();
    for(size_t i = 0; i < pRowCount; i++)
      data[i * mColCount + i] = 1;
    return true;
  }

  /** Return a pointer to the first element in the row pointed to by 'pIndex'. 
    * You have to make sure pIndex is smaller the mRowCount. No verification is done here.
    * First index value is 0. */
  T * operator[] (size_t pIndex)
  {
    return data + mColCount * pIndex;
  }
  
  /** Return a pointer to the first element in the row pointed to by 'pIndex'. 
    * You have to make sure pIndex is smaller the mRowCount. No verification is done here.
    * First index value is 0. */
  const T * operator[] (size_t pIndex) const
  {
    return data + mColCount * pIndex;
  }
  
  /** Return the element in the row pointed to by 'pRowIndex' at the column 'pColIndex'. 
    * You have to make sure indexes are valid. No verification is done here. */
  const T value_at (size_t pRowIndex, size_t pColIndex) const
  {
    return *(data + mColCount * pRowIndex + pColIndex);
  }
  
  /** Return a pointer to the data buffer. Return NULL if there is no data. */
  const T * raw_data() const
  {
    return data;
  }
  
  /** Return a pointer to the data buffer. Return NULL if there is no data. */
  T * raw_data()
  {
    return data;
  }
  
  /** Return the number of columns in the matrix. */
  const size_t row_count() const
  {
    return mRowCount;
  }
  
  /** Return the number of columns in the matrix. */
  const size_t col_count() const
  {
    return mColCount;
  }
  
  size_t size() const
  {
    return mRowCount * mColCount;
  }
  
public:
  T * data; /**< Pointer to the data. Public for performance, handle with care. */
  
protected:
  friend class CutMatrix;
  /** Manage memory allocation (make sure there is enough space for matrix of size pSize). 
    * @return false on memory allocation failure. */
  bool check_alloc(size_t pSize);
  
  /** Verify sizes before operations. */
  bool check_sizes(const char * pMsg, size_t * start_row, size_t * end_row, const TMatrix& pOther, int pStartRow, int pEndRow, bool pAllowColCountChange = false);

  /** The size of the matrix changed. We need to increase/decrease memory usage.
    * @return false on memory allocation failure. */
  bool reallocate(size_t pSize);  
  
  /** Copy data using memcpy. (Update size if needed).
    * @param pRowOffset where to start copying (set to mRowCount to append at end). 
    * @param pResize    size can reduce if this is true.
    */
  bool raw_copy(size_t pRowOffset, const T * pVector, size_t pVectorSize, bool pResize = false);
  
  /** Do multiplication (wrapper around Cblas) */
  inline void do_gemm(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA, const enum CBLAS_TRANSPOSE TransB, const int M, const int N, const int K, const real_t alpha, const T *A, const int lda, const T *B, const int ldb, const real_t beta, T *C, const int ldc);
  
  
  /** Read a char from a file. */
  inline char do_fscanf (FILE * pFile, char * pValue) const
  {
    return fscanf(pFile, " %c", pValue);
  }
  
  /** Read an integer from a file. */
  inline int do_fscanf (FILE * pFile, int * pValue) const
  {
    return fscanf(pFile, " %i", pValue);
  }
  
  /** Read a real_t from a file. */
  inline int do_fscanf (FILE * pFile, real_t * pValue) const
  {
    return fscanf(pFile, " %lf", pValue);
  }
  
  /** Write a char to a file. */
  inline void do_fprintf (FILE * pFile, char pValue) const
  {  
    fprintf(pFile, " %c", pValue);
  }
  
  /** Write an integer to a file. */
  inline void do_fprintf (FILE * pFile, int pValue) const
  {  
    fprintf(pFile, " %i", pValue);
  }

  /** Write a real_t to a file. */
  inline void do_fprintf (FILE * pFile, real_t pValue) const
  {  
    fprintf(pFile, " % .5f", pValue);
  }
  
  bool set_error(const char * fmt, ...);
  
  size_t mStorageSize; /**< Storage size is greater or equal to mRowCount * mColCount. */
  size_t    mRowCount; /**< Number of rows in the matrix. */
  size_t    mColCount; /**< Number of columns in the matrix. mRowCount x mColCount can be greater then mSize during partial append. */
  size_t  mPlaneCount; /**< Number of planes (for multiple-values cell content). All planes have the same size. */
  const char * mErrorMsg; /**< Pointer to the last error message. */
  char * mErrorBuffer; /**< Can be used to print custom messages. */
  size_t mErrorBufferSize;
};

typedef TMatrix< int  > IntMatrix;

typedef TMatrix<real_t> Matrix;


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
    // FIXME: update code to handle multiple planes and reference counting.
    // 1. alloc data pointers: data = new ...
    // 2. for(i=0;i<mPlaneCount;i++) data[i] = pOther.data[i];
    data = pOther.data + pStartRow * pOther.mColCount;
    mRowCount = end_row - pStartRow + 1;
    mColCount = pOther.mColCount;
  }
  
  CutMatrix(real_t * pData, size_t pRowCount, size_t pColCount) {
    data = pData;
    mRowCount = pRowCount;
    mColCount = pColCount;
  }
  
  CutMatrix() {}
  
  virtual ~CutMatrix()
  {
    data = NULL; // make sure it is never freed.
  }
  
  bool set_sizes(size_t pRowCount, size_t pColCount)
  {
    mRowCount = pRowCount;
    mColCount = pColCount;
    return true;
  }
  
  bool set_view(const Matrix& pOther, int pStartRow = 0, int pEndRow = -1)
  {
    if (!&pOther) {
      mRowCount = 0;
      mColCount = 0;
      data = NULL;
      return true;
    }
    
    if (pOther.mRowCount == 1) {
      // vector
      size_t end_col   = pEndRow   < 0 ? pOther.mColCount + pEndRow   : pEndRow;
      size_t start_col = pStartRow < 0 ? pOther.mColCount + pStartRow : pStartRow;
      if (start_col >= pOther.mColCount || start_col < 0) {
        set_error("size error (set_view): bad start column %i (%i) of vector %ix%i", pStartRow, start_col, pOther.mRowCount, pOther.mColCount);
        return false;
      } else if (end_col >= pOther.mColCount || end_col < 0) {
        set_error("size error (set_view): bad end column %i (%i) of vector %ix%i", pEndRow, end_col, pOther.mRowCount, pOther.mColCount);
        return false;
      }
      mRowCount = 1;
      mColCount = end_col - start_col + 1;
      data = pOther.data + start_col;
    } else {
      // matrix
      size_t end_row;   
      size_t start_row;
      if (!check_sizes("set_view", &start_row, &end_row, pOther, pStartRow, pEndRow, true)) return false;
      mRowCount = end_row - start_row + 1;
      mColCount = pOther.mColCount;
      data = pOther.data + start_row * mColCount;
    }
    return true;
  }
  
  void set_data(real_t * pData)
  {
    data = pData;
  }
};

template<typename T>
std::ostream& operator<< (std::ostream& pStream, const TMatrix<T>& mat);

#endif // _MATRIX_H_