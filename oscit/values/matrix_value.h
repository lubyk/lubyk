
//typedef struct {
//  rubyk_signal_t    type;
//  THash<uint, Value> * meta;
//  const Matrix * value;
//} MatrixValue_t;
//
#ifndef _MATRIX_VALUE_H_
#define _MATRIX_VALUE_H_
#include "value.h"
#include "tmatrix.h"

template<typename T>
class TMatrixx; // naming Matrixx to avoid name class with legacy Matrix. When Matrix is removed, this will be renamed.

/* Holds the actual data of the Matrix class. */
template<class T>
class TMatrixData : public Data, public TMatrix<T>
{
public:
  TMatrixData() : TMatrix<T>() {}
  
  TMatrixData(size_t pRowCount, size_t pColCount) : TMatrix<T>(pRowCount, pColCount) {}
  
  virtual Data * clone()
  { return new TMatrixData(*this); }
  
  virtual value_t type() const
  { return AnonymousValue; }         // Should be specialized in IntMatrix template.
  
  
  /** Display matrix inside stream. */
  virtual void to_stream(std::ostream& pStream) const;
  
  /** JSON representation of data into stream. */
  virtual void to_json(std::ostream& pStream) const;
  
protected:
  friend class TMatrixx<T>;
};

#define GET_DATA(x) const TMatrixData<T> * x ## _data = x.data(); \
  if (!x ## _data) return false;
#define GET_THIS_DATA() const TMatrixData<T> * this_data = data(); \
    if (!this_data) return false;
  
/** Value class to hold a matrix of elements of type 'T'. */
template<class T>
class TMatrixx : public Value
{
public:
  VALUE_METHODS(TMatrixx, TMatrixData<T>, AnonymousValue, Value)
  VALUE_FROM_STRING(TMatrixx)
  
  TMatrixx(size_t pRowCount, size_t pColCount) : Value(new TMatrixData<T>(pRowCount, pColCount)) {}
  
  const Value& operator= (const Value val)
  { 
    val.set(*this);
    return val; 
  }
  
  ///////////////// Proxy methods for TMatrix //////////
  
  /** Define and allocate the number of rows and columns for the matrix.
    * @return false on allocation failure. */
  bool set_sizes(size_t pRowCount, size_t pColCount)
  {
    return mutable_data()->set_sizes(pRowCount, pColCount);
  }
  
  /** Set all values to 'pVal'. */
  void fill(T pVal)
  {
    return mutable_data()->fill(pVal);
  }
  
  /** Set all the elements to 0. */
  void clear()
  {
    return mutable_data()->clear();
  }
  
  /** Write a matrix to a FILE pointer. 
      @param iMatrix if true, will write to return after write. Set to 
             false to record a partial matrix.
    */
  bool to_file(FILE * pFile, bool isMatrix = true) const
  {
    GET_THIS_DATA()
    return this_data->to_file(pFile, isMatrix);
  }
  
  /** Write a matrix to a filepath. */
  bool to_file(const std::string& pPath, const char * pMode = "wb", bool isMatrix = true) const
  {
    GET_THIS_DATA()
    return this_data->to_file(pPath, pMode, isMatrix);
  }
  
  /** Fill a matrix from a FILE pointer.
    * If the matrix has '0' rows, all the file is loaded into the matrix. If
    * the matrix has '0' columns, the size is built from the number of values until
    * the next newline.
    *
    * @return false if there was not enough values to fill the matrix. */
  bool from_file(FILE * pFile)
  {
    return mutable_data()->from_file(pFile);
  }
  
  /** Read a matrix from a filepath. */
  bool from_file(const std::string& pPath, const char * pMode = "rb")
  {
    return mutable_data()->from_file(pPath, pMode);
  }
  
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
    * @param A      other matrix to copy the data from.
    * @param pStartRow   where to start copying the data from (default is 0).
    * @param pEndRow     last row to copy (default is -1 = last row). */
  bool copy(const TMatrixx& A, int pStartRow = 0, int pEndRow = -1)
  {
    GET_DATA(A)
    return mutable_data()->copy_at(0, *A_data, pStartRow, pEndRow, true);
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
    * @param A      other matrix to copy the data from.
    * @param pStartRow   where to start copying the data from (default is 0).
    * @param pEndRow     last row to copy (default is -1 = last row).
    * @param pResize     if true and pRowIndex is '0', the matrix is resized to the copied data.
    *
    * @return bool       returns false if allocation of new space failed. */
  bool copy_at(int pRowIndex, const TMatrixx& A, int pStartRow = 0, int pEndRow = -1, bool pResize = false)
  {
    GET_DATA(A)
    return mutable_data()->copy_at(pRowIndex, *A_data, pStartRow, pEndRow, pResize);
  }

  // FIXME: what is this ? bool copy(const Value val);
  
  // FIXME: what is this ? bool copy_at(int pRowIndex, const Value val);
  
  /** Append a vector to the end of the current data. Size increases automatically. */
  bool append (const T * pVector, size_t pVectorSize)
  {
    return mutable_data()->append(pVector, pVectorSize);
  }
  
  /** Cast & append a vector. Size increases automatically. */
  // template<typename V>
  // bool cast_append (const V * pVector, size_t pVectorSize, real_t pScale = 1.0);
  
  /** Append another matrix/vector to the end of the current data. Size increases automatically. 
    * @param pStartRow   where to start copying the data from (default is 0).
    * @param pEndRow     last row to copy (default is -1 = last row).
    * @return false if the column count of both matrices do not match. */
  bool append (const TMatrixx& A, int pStartRow = 0, int pEndRow = -1)
  {
    GET_DATA(A)
    return mutable_data()->append(*A_data, pStartRow, pEndRow);
  }
  
  /** Append a value at the end of a vector. Size increases automatically. */
  bool append (T pValue)
  {
    return mutable_data()->append(pValue);
  }
  
  
  /** Add elements of one matrix to another.
    * If rows/columns match, elements are added one by one.
    * If the other matrix is a vector and column sizes match, the vector is added to each row.
    * If the other matrix is a column vector and row counts match, the scalar is used against the corresponding row.
    * If the other matrix is a scalar, add the value to all elements.
    *
    * @param A other matrix whose elements will be added.
    * @param pStartRow if you want to use only part of the other matrix, start row. Default 0 (first row).
    * @param pEndRow   when using only part of the other matrix. Default -1 (last row).
    * @param pScale    amount to multiply each value before adding.
    *
    * @return true (never fails). */
  bool add (const TMatrixx& A, int pStartRow = 0, int pEndRow = -1, real_t pScale = 1.0)
  {
    GET_DATA(A)
    return mutable_data()->add(*A_data, pStartRow, pEndRow, pScale);
  }
  
  /** Subtract elements of one matrix to another.
    * See 'add' for details. */
  inline bool subtract(const TMatrixx& A, int pStartRow = 0, int pEndRow = -1, real_t pScale = 1.0)
  { 
    GET_DATA(A)
    return mutable_data()->add(*A_data, pStartRow, pEndRow, -pScale);
  }
  
  /** Add an array of reals to each elements in the matrix. 
    * If the size is the same as the matrix : one to one.
    * If the size is col_size : add to each row.
    * If the size is row_size : add corresponding value to element in the row. */
  bool add (const T * pVector, size_t pVectorSize)
  {
    return mutable_data()->add(pVector, pVectorSize);
  }
  
  /** Add two matrices. */
  bool add (const TMatrixx& A, const TMatrixx& B, real_t pScaleA = 1.0, real_t pScaleB = 1.0)
  {
    GET_DATA(A)
    GET_DATA(B)
    return mutable_data()->add(*A_data, *B_data, pScaleA, pScaleB);
  }
  
  /** Divide all elements by the values in another matrix. a.divide(b) (a/b) is NOT the matrix division (a-1b)
    * If rows/columns match, elements are divided one by one.
    * If the other matrix is a vector and columns sizes match, each row is divided by the vector.
    * If the other matrix is a column vector and row counts match, corresponding rows are divided by the scalar.
    * If the other matrix is a scalar, divide all element by this value.
    *
    * @param A other matrix by which the elements of this matrix will be divided.
    * @param pStartRow if you want to use only part of the other matrix, start row. Default 0 (first row).
    * @param pEndRow   when using only part of the other matrix. Default -1 (last row).
    * @return true (never fails). */
  bool divide (const TMatrixx& A, int pStartRow = 0, int pEndRow = -1, real_t pScale = 1.0)
  {
    GET_DATA(A)
    return mutable_data()->divide(*A_data, pStartRow, pEndRow, pScale);
  }
  
  /** Divide all elements by the values in another matrix.
    * See 'divide' for details. */
  bool operator/= (const TMatrixx& A)
  {
    GET_DATA(A)
    return mutable_data()->divide(*A_data);
  }
  
  /** Divide all elements by a scalar.
    *
    * @param pValue  value by which to divide all elements of the current matrix.
    *
    * @return true (never fails). */
  bool operator/= (T pValue)
  {
    (*mutable_data()) /= pValue;
    return true;
  }
  
  /** Multiply all elements by the values in another matrix. a.divide(b) (a/b) is NOT the matrix multiplication (ab). See 'mat_multiply'.
    * If rows/columns match, elements are multiplied one by one.
    * If the other matrix is a vector and columns sizes match, each row is multiplied by the vector.
    * If the other matrix is a column vector and row counts match, corresponding rows are multiplied by the scalar.
    * If the other matrix is a scalar, multiply all element by this value.
    *
    * @param A other matrix by which the elements of this matrix will be divided.
    * @param pStartRow if you want to use only part of the other matrix, start row. Default 0 (first row).
    * @param pEndRow   when using only part of the other matrix. Default -1 (last row).
    * @return true (never fails). */
  bool multiply (const TMatrixx& A, int pStartRow = 0, int pEndRow = -1, real_t pScale = 1.0)
  {
    GET_DATA(A)
    return mutable_data()->multiply(*A_data, pStartRow, pEndRow, pScale);
  }
  
  /** Divide all elements by the values in another matrix.
    * See 'divide' for details. */
  bool operator*= (const TMatrixx& A)
  {
    GET_DATA(A)
    return mutable_data()->multiply(*A_data);
  }
  
  /** Divide all elements by a scalar.
    *
    * @param pValue  value by which to divide all elements of the current matrix.
    *
    * @return true (never fails). */
  bool operator*= (T pValue)
  {
    (*mutable_data()) *= pValue;
    return true;
  }
  
  /** Substract all elements by the values in another matrix.
    * See 'add' for details. */
  bool operator-= (const TMatrixx& A)
  {
    GET_DATA(A)
    return mutable_data->add(*A_data, 0, -1, -1.0);
  }
  
  /** Substract a value to all elements in the matrix. */
  bool operator-= (T pValue)
  {
    (*mutable_data()) -= pValue;
    return true;
  }
  
  /** Add elements of another matrix. See 'add' for details. */
  bool operator+= (const TMatrixx& A)
  {
    GET_DATA(A)
    return mutable_data()->add(*A_data);
  }
  
  /** Add a value to all elements in the matrix. */
  bool operator+= (T pValue)
  {
    (*mutable_data()) += pValue;
    return true;
  }
  
  /** Return the message for the last error. */
  const char * error_msg() const
  {
    return mPtr ? data_pointer()->mErrorMsg : "";
  }
  
  /** Print the matrix (usefull for debugging). Use 'to_file' to serialize. */
  void print(FILE * pFile = stdout) const
  {
    GET_THIS_DATA()
    this_data->print(pFile);
  }
  
  ////////////////  MATRIX OPERATIONS ////////////////////////
  /** Matrix multiplication.
    * Write C.mat_multiply(A, B) for C = AB
    *
    * @param A matrix A.
    * @param B matrix B.
    * @param pTransA transposition mode for matrix A (CblasNoTrans/CblasTrans).
    * @param pTransB transposition mode for matrix B.
    * @param pScale  scale factor. Default is 1.0 (no scaling). */
  bool mat_multiply(const TMatrixx& A, const TMatrixx& B, const enum CBLAS_TRANSPOSE pTransA = CblasNoTrans, const enum CBLAS_TRANSPOSE pTransB = CblasNoTrans, real_t pScale = 1.0)
  {
    GET_DATA(A)
    GET_DATA(B)
    return mutable_data()->mat_multiply(*A_data, *B_data, pTransA, pTransB, pScale);
  }
  
  
  /** Compute A'A for the given (row major) matrix. Return false on failure. Write C.symmetric(A) for C = A'A. */
  bool symmetric(const TMatrixx& A)
  {
    GET_DATA(A)
    return mutable_data()->symmetric(*A_data);
  }

  /** Set the matrix to the eigenvectors of a symmetric (row major) matrix. Return false on failure.
    * @param pEigenValues will contain the eigenvalues (in ascending order).
    * @param pMatrix source symmetric matrix (will be altered during processing. Send a copy if you want it kept clean).
    */
  bool eigenvectors(TMatrixx& pEigenValues, TMatrixx& pMatrix)
  {
    TMatrixData<T> * pEigenValues_data = pEigenValues.mutable_data();
    if (!pEigenValues_data) return false;
    
    TMatrixData<T> * pMatrix_data = pMatrix.mutable_data();
    if (!pMatrix_data) return false;
    
    return mutable_data()->eigenvectors(*pEigenValues_data, *pMatrix_data);
  }
  
  
  /** Compute in-place inverse of a square matrix.
    * @eturn false on failure.
    */
  bool inverse()
  {
    if (!data()) return false;
    return mutable_data()->inverse();
  }
  
  /** Create an identity matrix of size 'n'.
    * @param n size of identity matrix. */
  bool identity(size_t pRowCount)
  {
    return mutable_data()->identity(pRowCount);
  }

  
  /** Return a pointer to the first element in the row pointed to by 'pIndex'. 
    * You have to make sure pIndex is smaller the mRowCount. No verification is done here. */
  T * operator[] (size_t pIndex)
  {
    return (*data_pointer())[pIndex];
  }
  
  /** Return a pointer to the first element in the row pointed to by 'pIndex'. 
    * You have to make sure pIndex is smaller the mRowCount. No verification is done here. */
  const T * operator[] (size_t pIndex) const
  {
    return (*data_pointer())[pIndex];
  }
  
  /** Return the element in the row pointed to by 'pRowIndex' at the column 'pColIndex'. 
    * You have to make sure indexes are valid. No verification is done here. */
  const T value_at (size_t pRowIndex, size_t pColIndex) const
  {
    return mPtr ? data_pointer()->value_at(pRowIndex, pColIndex) : 0;
  }
  
  /** Return a pointer to the data buffer. Return NULL if there is no data. */
  const T * raw_data() const
  {
    return mPtr ? data_pointer()->data : NULL;    
  }
  
  /** Return a pointer to the data buffer. Return NULL if there is no data. */
  T * raw_data()
  {
    return mPtr ? data_pointer()->data : NULL; // FIXME: shouldn't we use mutable_data() here ?
  }
  
  /** Return the number of columns in the matrix. */
  const size_t row_count() const
  {
    return mPtr ? data_pointer()->mRowCount : 0;
  }
  
  /** Return the number of columns in the matrix. */
  const size_t col_count() const
  {
    return mPtr ? data_pointer()->mColCount : 0;
  }
  
  size_t size() const
  {
    return mPtr ? data_pointer()->size() : 0;
  }
  
  //////////////////////////////////////////////////////
  
  
};

/////// real_t ///////

typedef TMatrixData<real_t> MatrixData;
typedef TMatrixx<real_t>    Matrixx;

template<>
inline value_t TMatrixData<real_t>::type() const
{ return MatrixValue; }

template<>
inline value_t TMatrixx<real_t>::type() const
{ return MatrixValue; }


/////// char ///////

typedef TMatrixData<char> CharMatrixData;
typedef TMatrixx<char>    CharMatrix;

template<>
inline value_t TMatrixData<char>::type() const
{ return CharMatrixValue; }

template<>
inline value_t TMatrixx<char>::type() const
{ return CharMatrixValue; }


#undef GET_THIS_DATA
#undef GET_DATA
#endif // _MATRIX_VALUE_H_