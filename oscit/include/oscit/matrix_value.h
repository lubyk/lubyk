#ifndef OSCIT_INCLUDE_OSCIT_MATRIX_VALUE_H_
#define OSCIT_INCLUDE_OSCIT_MATRIX_VALUE_H_
#include "oscit/value.h"
#include "oscit/matrix.h"

namespace oscit {

/** Value is the base type of all data transmitted between objects or used as parameters
and return values for osc messages.*/
class MatrixValue : public Value
{ 
 public: 
  explicit MatrixValue() {
    set_type(MATRIX_VALUE);
  }
  MatrixValue(size_t rows, size_t cols) : Value(Matrix(rows,cols)) {}
  explicit MatrixValue(const Matrix &matrix) : Value(matrix) {}
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_MATRIX_VALUE_H_
