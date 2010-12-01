/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

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

#include "test_helper.h"
#include "oscit/matrix.h"

static float cv_mat_test_data[28]  = { 9, 9, 9, 9, 9, 9, 9,  // 9 = padding
                                       9, 1, 0, 0, 0, 9, 9,  // 3x4
                                       9, 0, 2, 3, 0, 9, 9,  //
                                       9, 0, 0, 0, 4, 9, 9}; //
class CvMatTest : public TestHelper {
public:
  void test_copy_userdata( void ) {
    float *raw_data = (float*)malloc(28 * sizeof(float));
    for (size_t i=0; i<28; ++i) raw_data[i] = cv_mat_test_data[i];

    Value val(4, 7, CV_32FC1, raw_data);

    // remove padding by setting Region of interest (ROI):
    val.matrix_->adjustROI(-1, 0, -1, -2); // top, bottom, left, right

    assert_equal(3, val.matrix_->rows);
    assert_equal(4, val.matrix_->cols);

    Matrix mat_B;
    val.matrix_->copyTo(mat_B);

    assert_equal(3, mat_B.rows);
    assert_equal(4, mat_B.cols);

    free(raw_data);

    float *data = (float*)mat_B.data;
    assert_equal(1.0, data[0]);

    data[0] = 2.4; // should not try to access raw_data
  }

  void test_resize_userdata( void ) {
    Matrix mat_A(4, 7, CV_32FC1, cv_mat_test_data);
    mat_A.adjustROI(-1, 0, -1, -2);

    Matrix mat_B(3, 2, CV_32FC1);

    cv::resize(mat_A, mat_B, cv::Size(2, 3));

    assert_equal(3, mat_B.rows);
    assert_equal(2, mat_B.cols);

    float *B_data = (float *)mat_B.data;

    assert_equal(0.5, B_data[0]);
    assert_equal(1.5, B_data[3]);
  }
};









