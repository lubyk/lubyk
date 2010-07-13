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

#ifndef RUBYK_INCLUDE_RUBYK_SUPPORT_DRAW_H_
#define RUBYK_INCLUDE_RUBYK_SUPPORT_DRAW_H_

#include "opencv/cv.h"

/** Main namespace for rubyk.
 */
namespace rk {

/** Draw a CV_8UC3  matrix (setting colors from matrix content).
 * Obsolete, use textures instead (@ref makeTexture, @ref drawTexture)
 * @param mat matrix containing the pixel colors
 * @param start_x left border of the rectangle in which to draw the matrix
 * @param start_y bottom of the rectangle in which to draw the matrix
 * @param end_x right border of the rectangle in which to draw the matrix
 * @param end_y top border of the rectangle in which to draw the matrix
 * @param z_mul use luminosity as z index (do not use: will be removed)
 */
void drawMatrix(const cv::Mat &mat, float start_x, float start_y, float end_x, float end_y, float alpha = 1.0, float z_mul = 0, bool strips = false);

/** Create a texture from a matrix.
 * @param mat matrix containing the data to be projected on surfaces
 * @param tex a texture id returned by glGenTextures
 */
void makeTexture(const cv::Mat &mat, int tex);

/** Draw a given texture on a square rectangle.
 * Do not forget to set a color before drawing the texture (usually full white).
 * @param tex a texture id returned by glGenTextures on which matrix data has been projected with @ref makeTexture
 * @param start_x left border of the rectangle in which to draw the texture
 * @param start_y bottom of the rectangle in which to draw the texture
 * @param end_x right border of the rectangle in which to draw the texture
 * @param end_y top border of the rectangle in which to draw the texture
 * @param flip_y (optional) flip image left-right (true = flip, false = no flip)
 */
void drawTexture(int tex, float start_x, float start_y, float end_x, float end_y, bool flip_y = false);

} // rk

/** Additions to cv namespace
 */
namespace cv {

/** Get value at the given position (eventually specifying the channel).
 * @param x column
 * @param y row
 * @param c channel (not used for 1 channel images)
 * @return float value at the position (returns 0 for out-of-bound values)
 */
inline float rk_mat_at(const cv::Mat &m, int x, int y, int c=0) {
  if ( y < 0 || y >= m.rows || x < 0 || x >= m.cols || c < 0 || c >= m.channels()) return 0;
  switch (m.depth()) {
    case CV_8U:  return ((const unsigned char* )(m.data + m.step*y))[x];
    case CV_32F: return ((const float*)(m.data + m.step*y))[x];
    default:
      return 0; // FIXME get other types
  }
}
} // cv

/** Additions to glu namespace that uses our Matrix class
 */
namespace glu {
/** Build a mipmap from a Matrix.
 * @param mat matrix (image) containing the data (must be a CV_8UC3)
 */
void Build2DMipmaps(const cv::Mat &mat);
} // glu


/** Additions to gl namespace that uses our Matrix class
 */
namespace gl {

/** Draw matrix pixels.
 * @param mat matrix (image) containing the data (must be a CV_8UC3)
 */
void DrawPixels(const cv::Mat &mat);

/** Load a matrix into the current texture.
 * @param mat the matrix that will be drawn on the texture.
 */
void TexImage(const cv::Mat &mat);

} // gl

namespace cv {
  cv::Mat LoadImage(const char *path);
}

#endif // RUBYK_INCLUDE_RUBYK_SUPPORT_DRAW_H_