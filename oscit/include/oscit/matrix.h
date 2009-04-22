#ifndef _OSCIT_MATRIX_H_
#define _OSCIT_MATRIX_H_
#include "oscit/values.h"
#include "opencv/cv.h"
#include <vector>

namespace oscit {

/** 
 * cv::Mat is defined in cxcore.hpp:686
 */
class Matrix : public cv::Mat
{
 public: 
  Matrix() {}
#if Real == double
  Matrix(int rows, int cols) : cv::Mat( rows, cols, CV_64FC1 ) {}
#else
  Matrix(int rows, int cols) : cv::Mat( rows, cols, CV_32FC1 ) {}
#endif
  Matrix(int rows, int cols, int type) : cv::Mat(rows, cols, type) {}
};

} // oscit
#endif // _OSCIT_MATRIX_H_