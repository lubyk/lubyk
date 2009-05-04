#ifndef OSCIT_INCLUDE_OSCIT_MATRIX_H_
#define OSCIT_INCLUDE_OSCIT_MATRIX_H_
#include <vector>

#include "oscit/values.h"

namespace oscit {

/** 
 * cv::Mat is defined in cxcore.hpp:686
 */
class Matrix {
 public: 
  Matrix();
  Matrix(int rows, int cols);
  Matrix(int rows, int cols, int type);
  Matrix(const Matrix &other);
  
  ~Matrix();
  
  int type();
  
  size_t rows();
  
  size_t cols();
  
  void *data();
  
 private:
  class Implementation;
  Implementation *impl_;
};

} // oscit
#endif // OSCIT_INCLUDE_OSCIT_MATRIX_H_