
//typedef struct {
//  rubyk_signal_t    type;
//  Hash<uint, Signal> * meta;
//  const Matrix * value;
//} MatrixSignal_t;
//
#ifndef _MATRIX_VALUE_H_
#define _MATRIX_VALUE_H_
#include "value.h"

class Matrixx; // naming Matrixx to avoid name class with legacy Matrix. When Matrix is removed, this will be renamed.

/* Holds the actual data of the Number class. This is a wrapper around a double. */
class MatrixData : public Data, public TMatrix<double>
{
public:
  MatrixData() : TMatrix<double>() {}
  
  MatrixData(size_t pRowCount, size_t pColCount) : TMatrix<double>(pRowCount, pColCount) {}
  
  virtual Data * clone()
  { return new MatrixData(*this); }
  
  virtual value_t type() const
  { return MatrixValue; }
};

/** Value class to hold a single number (double). */
class Matrixx : public Value
{
public:
  VALUE_METHODS(Matrixx, MatrixData, MatrixValue)
  
  Matrixx(size_t pRowCount, size_t pColCount) : Value(new MatrixData(pRowCount, pColCount)) {}
  
  const double * raw_data () const
  {
    if (mPtr) {
      return ((MatrixData*)mPtr->mDataPtr)->data;
    } else {
      return NULL;
    }
  }
  
};

#endif // _MATRIX_VALUE_H_