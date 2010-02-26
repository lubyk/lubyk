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
#include "rubyk/opengl.h"
#include "oscit/matrix.h"

class GLMat : public Node {
public:
  GLMat() : drawing_(false) {}

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
    }

    draw();
  }

  // [2] set matrix inlet
  void matrix(const Value &val) {
    ScopedLock lock(drawing_flag_); // forbid change in drawing_ value
    if (drawing_) {
      std::cout << "drop frame\n";
      return;
    }

    if (val.is_matrix()) {
      if (val.matrix_->type() != CV_8UC3) {
        std::cerr << "Cannot display matrix (type should be CV_8UC3)\n";
        return;
      }

      if (!val.matrix_->cols || !val.matrix_->rows) {
        std::cerr << "Cannot display 0x0 matrix\n";
        return;
      }
      std::cout << "[ Need copy\n";
      val.matrix_->copyTo(copied_frame_);

      cv::resize(*val.matrix_, copied_frame_, cv::Size(width_/10, height_/10), 0, 0, CV_INTER_NN);

      std::cout << "  Copied ]\n";
    }
  }

protected:
  void draw() {
    { ScopedLock lock(drawing_flag_);
      drawing_ = true;
    }
    if (copied_frame_.cols && copied_frame_.rows) {

      std::cout << "[ Draw \n";
      size_t rows = copied_frame_.rows;
      size_t cols = copied_frame_.cols;
      size_t row_step = copied_frame_.step1();

      float x1, y1;
      float pix_width  = width_  / cols;
      float pix_height = height_ / rows;

      GLuint *color;
      GLuint *row_start;
      GLuint *data = (GLuint*)copied_frame_.data;


      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      //glTranslatef(0.0, 0.0, -4.0);

      //glColor3f(1.0, 0.0, 0.0);
      //glRectf(0, 0, width_, height_);
      for (size_t j = 0; j < rows; ++j) {
        row_start = data + j * row_step;
        y1 = j * pix_height;
        for (size_t i = 0; i < cols; ++i) {
          color = row_start + 3 * i;
          x1    = pix_width * i;
          glColor3uiv(color);
          glRectf(
            x1,
            y1,
            x1 + pix_width,
            y1 + pix_height
          );
        }
      }
      std::cout << "  draw done ]\n";
    }

    { ScopedLock lock(drawing_flag_);
      drawing_ = false;
    }
  }

  void resize(Real width, Real height) {
    std::cout << " Resize\n";
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
  Mutex drawing_flag_;
  /** Tell the incomming matrix to drop frame because we
   * haven't finished drawing.
   */
  bool drawing_;

  /** View's width in pixels.
   */
  double width_;

  /** View's height in pixels.
   */
  double height_;
};

extern "C" void init(Planet &planet) {
  CLASS (GLMat, "GLMat displays a Matrix on an OpenGL window.", "")
  // [1]
  INLET (GLMat, draw, Value(Json("[0,0]")).push_back("Receives [width, height] from an OpenGL thread."))
  // [2]
  INLET (GLMat, matrix, MatrixIO(0, 0, "Receives a matrix to be drawn on screen."))
}
