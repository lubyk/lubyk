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

#include "rubyk.h"
using namespace rk;

#include "rubyk/opengl.h"
#include "oscit/matrix.h"

class GLMat : public Node {
public:

  const Value init() {
    downsample_ = 4.0;
    return gNilValue;
  }

  // [1] draw inlent
  void draw(const Value &val) {
    if (!is_opengl_thread()) {
      fprintf(stderr, "%s: cannot call draw (not an OpenGL thread)\n", name_.c_str());
      return;
    }

    if (val[0].r != width_ || val[1].r != height_) {
      width_  = val[0].r;
      height_ = val[1].r;
      resize(width_, height_);
      { ScopedWrite lock(size_lock_);
        size_ = cv::Size(width_ / downsample_, height_ / downsample_);
      }
    }

    draw();
  }

  // [2] set matrix inlet
  void matrix(const Value &val) {
    ScopedRead lock(size_lock_); // forbid change in drawing_ value

    if (val.is_matrix()) {
      if (val.matrix_->type() != CV_8UC3) {
        std::cerr << "Cannot display matrix (type should be CV_8UC3)\n";
        return;
      }

      if (!val.matrix_->cols || !val.matrix_->rows) {
        std::cerr << "Cannot display 0x0 matrix\n";
        return;
      }
      if (copied_frame_.size() != size_) {

        copied_frame_.create(size_, val.matrix_->type());

        /** This is a loop test (fill the matrix with a gradient).
        unsigned char *data = (unsigned char *)copied_frame_.data;
        size_t row_step = copied_frame_.step1();
        size_t channels = copied_frame_.channels();

        for (size_t j=0; j<size_.height; ++j) {
          unsigned char row_p = (j * 127) / size_.height;
          unsigned char *start_row = data + j * row_step;
          for (size_t i=0; i<size_.width; ++i) {
            start_row[(i * channels) + 0] = row_p + (i * 127) / size_.width;
            start_row[(i * channels) + 1] = row_p + (i * 127) / size_.width;
            start_row[(i * channels) + 2] = row_p + (i * 127) / size_.width;
          }
        }
        */
      }
      cv::resize(*val.matrix_, copied_frame_, size_, 0, 0, CV_INTER_NN);
    }
  }

  // {downsample} Downsample factor
  const Value downsample(const Value &val) {
    if (val.is_real() && val.r >= 1) {
      { ScopedWrite lock(size_lock_);
        downsample_ = val.r;
        size_ = cv::Size(width_ / downsample_, height_ / downsample_);
      }
    }

    return Value(downsample_);
  }

protected:
  void draw() {
    ScopedRead lock(size_lock_);
    draw_matrix(copied_frame_, 0, height_, width_, 0);
  }

  /** Draw a matrix in the rectangle defined by the start and end coordinates.
   */
  void draw_matrix(const Matrix &mat, float start_x, float start_y, float end_x, float end_y) {
    if (mat.cols && mat.rows) {
      size_t rows = mat.rows;
      size_t cols = mat.cols;
      size_t row_step = mat.step1();

      float x1, y1;
      float pix_width  = (end_x - start_x) / cols;
      float pix_height = (end_y - start_y) / rows;
      unsigned char *color;
      unsigned char *row_start;
      unsigned char *data = (unsigned char *)mat.data;


      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      // glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, 256, 256, GL_RGB, GL_UNSIGNED_BYTE, data);

      for (size_t j = 0; j < rows; ++j) {
        row_start = data + j * row_step;
        y1 = start_y + j * pix_height;
        for (size_t i = 0; i < cols; ++i) {
          color = row_start + 3 * i;
          x1    = start_x + pix_width * i;
          glColor3f(
            color[0] / 255.0,
            color[1] / 255.0,
            color[2] / 255.0
          );
          glRectf(
            x1,
            y1,
            x1 + pix_width,
            y1 + pix_height
          );
        }
      }
    }
  }

  void resize(Real width, Real height) {
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    // Select the projection matrix
    glMatrixMode(GL_PROJECTION);
    // reset
    glLoadIdentity();

    gluOrtho2D(0, width, 0, height);

    // Select the modelview matrix
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    height_ = height;
    width_  = width;
  }

  Matrix copied_frame_;

  /** Protect size altering.
   */
  RWMutex size_lock_;

  /** View's width in pixels.
   */
  Real width_;

  /** View's height in pixels.
   */
  Real height_;

  /** Resized matrix dimension.
   */
  cv::Size size_;

  /** Downsample factor.
   */
  Real downsample_;
};

extern "C" void init(Planet &planet) {
  CLASS (GLMat, "GLMat displays a Matrix on an OpenGL window.", "")
  // [1]
  // TODO: send 'gl object' ? (void pointer)
  METHOD(GLMat, draw, Attribute::io("Receives [width, height] from an OpenGL thread.", "opengl", "ff"))
  // [2]
  METHOD(GLMat, matrix, Attribute::matrix_io("Receives a matrix to be drawn on screen."))
  // {downsample}
  METHOD(GLMat, downsample, Attribute::range_io("Downsampling factor.", 1, 64))
}
