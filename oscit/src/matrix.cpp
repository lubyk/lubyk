/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#include "oscit/values.h"

#include <vector>

#include "opencv/cv.h"


namespace oscit {

/**
 * cv::Mat is defined in cxcore.hpp:686
 */
struct Matrix::Implementation : public cv::Mat {
  Implementation() {}
  Implementation(int rows, int cols, int type) : cv::Mat(rows, cols, type) {}

  bool operator==(const Matrix::Implementation &other) {
    // FIXME: this is all wrong but who really needs to compare matrices anyway ?
    // This implementation is just here so that we can compare Values.
    std::cerr << __FILE__ << ":" << __LINE__ << " WARNING: comparing matrices is not implemented, it might return wrong results\n";
    return rows == other.rows && cols == other.cols;
  }
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

const cv::Mat *Matrix::cv_mat() {
  return impl_;
}

bool Matrix::operator==(const Matrix &other) const {
  return *impl_ == *other.impl_;
}

} // oscit

// opencv
#include "opencv/cxalloc.cpp"
#include "opencv/cxsystem.cpp"
