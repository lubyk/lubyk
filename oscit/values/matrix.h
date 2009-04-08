#ifndef _MATRIX_H_
#define _MATRIX_H_
#include "oscit/values.h"
#include "opencv/cxtypes.h"
#include <vector>

namespace oscit {
  
enum ValueType
{
  NIL_VALUE = 0,
  REAL_VALUE,
  STRING_VALUE,
  ERROR_VALUE,
  LIST_VALUE,
  HASH_VALUE,
};

class Matrix : public CvMatrix
{
 public: 
#if Real == double
  Matrix(int rows, int cols) : CvMatrix( rows, cols, CV_64FC1 ) {}
#else
  Matrix(int rows, int cols) : CvMatrix( rows, cols, CV_32FC1 ) {}
#endif
  Matrix(int rows, int cols, int type) : CvMatrix(rows, cols, type) {}
};

} // oscit
#endif // _MATRIX_H_