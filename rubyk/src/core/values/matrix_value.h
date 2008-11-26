
//typedef struct {
//  rubyk_signal_t    type;
//  Hash<uint, Signal> * meta;
//  const Matrix * value;
//} MatrixSignal_t;
//
#ifndef _MATRIX_VALUE_H_
#define _MATRIX_VALUE_H_
#include "value.h"

template<class T>
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
protected:
  friend class TMatrixx<T>;
};

/** Value class to hold a matrix of elements of type 'T'. */
template<class T>
class TMatrixx : public Value
{
public:
  VALUE_METHODS(TMatrixx, TMatrixData<T>, AnonymousValue)
  
  TMatrixx(size_t pRowCount, size_t pColCount) : Value(new TMatrixData<T>(pRowCount, pColCount)) {}
  
  const T * raw_data () const
  {
    if (mPtr) {
      return data_pointer()->data;
    } else {
      return NULL;
    }
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
    const TMatrixData<T> * d = data();
    if (!d) return false;
    return d->to_file(pFile, isMatrix);
  }
  
  /** Write a matrix to a filepath. */
  bool to_file(const std::string& pPath, const char * pMode = "wb", bool isMatrix = true) const
  {
    const TMatrixData<T> * d = data();
    if (!d) return false;
    return d->to_file(pPath, pMode, isMatrix);
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
    * @param pOther      other matrix to copy the data from.
    * @param pStartRow   where to start copying the data from (default is 0).
    * @param pEndRow     last row to copy (default is -1 = last row). */
  bool copy(const TMatrixx& pOther, int pStartRow = 0, int pEndRow = -1)
  {
    const TMatrixData<T> * other_data = pOther.data();
    if (!other_data) return false;
    return mutable_data()->copy_at(0, *other_data, pStartRow, pEndRow, true);
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
  bool copy_at(int pRowIndex, const TMatrixx& pOther, int pStartRow = 0, int pEndRow = -1, bool pResize = false)
  {
    const TMatrixData<T> * other_data = pOther.data();
    if (!other_data) return false;
    return mutable_data()->copy_at(pRowIndex, *other_data, pStartRow, pEndRow, pResize);
  }

  // FIXME: what is this ? bool copy(const Signal& sig);
  
  // FIXME: what is this ? bool copy_at(int pRowIndex, const Signal& sig);
  
  /** Append a vector to the end of the current data. Size increases automatically. */
  bool append (const T * pVector, size_t pVectorSize)
  {
    return mutable_data()->append(pVector, pVectorSize);
  }
  
  /** Cast & append a vector. Size increases automatically. */
  // template<typename V>
  // bool cast_append (const V * pVector, size_t pVectorSize, double pScale = 1.0);
  
  /** Append another matrix/vector to the end of the current data. Size increases automatically. 
    * @param pStartRow   where to start copying the data from (default is 0).
    * @param pEndRow     last row to copy (default is -1 = last row).
    * @return false if the column count of both matrices do not match. */
  bool append (const TMatrixx& pOther, int pStartRow = 0, int pEndRow = -1)
  {
    const TMatrixData<T> * other_data = pOther.data();
    if (!other_data) return false;
    return mutable_data()->append(*other_data, pStartRow, pEndRow);
  }
  
  /** Append a value at the end of a vector. Size increases automatically. */
  bool append (T pValue)
  {
    return mutable_data()->append(pValue);
  }
  
  //////////////////////////////////////////////////////
  
  size_t row_count()
  {
    return mutable_data()->mRowCount;
  }
  
  size_t col_count()
  {
    return mutable_data()->mColCount;
  }
  
  size_t size()
  {
    return mPtr ? data_pointer()->size() : 0;
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
  const T& value_at (size_t pRowIndex, size_t pColIndex) const
  {
    return mPtr ? data_pointer()->value_at(pRowIndex, pColIndex) : 0;
  }
  
};


template<>
value_t TMatrixData<double>::type() const
{ return MatrixValue; }

template<>
value_t TMatrixx<double>::type() const
{ return MatrixValue; }

template<>
value_t TMatrixData<char>::type() const
{ return CharMatrixValue; }

template<>
value_t TMatrixx<char>::type() const
{ return CharMatrixValue; }

typedef TMatrixx<double> Matrixx;
typedef TMatrixData<double> MatrixData;

#endif // _MATRIX_VALUE_H_