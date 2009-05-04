#ifndef _OSCIT_MATRIX_H_
#define _OSCIT_MATRIX_H_
#include "oscit/values.h"
#include "opencv/cv.h"
#include <vector>


namespace oscit {

/** 
 * cv::Mat is defined in cxcore.hpp:686
 */
struct Matrix::Implementation : public cv::Mat {
  Implementation() {}
  Implementation(int rows, int cols, int type) : cv::Mat(rows, cols, type) {}
};

Matrix::Matrix() {
  impl_ = new Matrix::Implementation();
}

Matrix::~Matrix() {
  delete impl_;
}

Matrix::Matrix(int rows, int cols) {
#if Real == double
  impl_ = new Matrix::Implementation(rows, cols, CV_64FC1 );
#else
  impl_ = new Matrix::Implementation(rows, cols, CV_32FC1 );
#endif
}

Matrix::Matrix(int rows, int cols, int type) {
  impl_ = new Matrix::Implementation(rows, cols, type);
}

Matrix::Matrix(const Matrix &other) {
  impl_ = new Matrix::Implementation(*other.impl_);
}

size_t Matrix::rows() {
  return impl_->rows;
}

size_t Matrix::cols() {
  return impl_->cols;
}

void *Matrix::data() {
  return impl_->data;
}

int Matrix::type() {
  return impl_->type();
}

} // oscit

// opencv
#include "opencv/cxalloc.cpp"
#include "opencv/cxsystem.cpp"

#endif // _OSCIT_MATRIX_H_