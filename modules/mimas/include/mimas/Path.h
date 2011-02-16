/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
   Copyright (c) 2007-2011 by Gaspard Bucher (http://teti.ch).

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
#ifndef LUBYK_INCLUDE_MIMAS_PATH_H_
#define LUBYK_INCLUDE_MIMAS_PATH_H_

#include "mimas/mimas.h"
#include <QtGui/QPainterPath>

#include <iostream>

namespace mimas {

/** Pen used by Painter.
 *
 */
class Path : public QPainterPath
{
public:
  Path() {}

  ~Path() {
    MIMAS_DEBUG_GC
  }

  void moveTo(float x, float y) {
    QPainterPath::moveTo(x, y);
  }

  /** Bezier curve from current point to endPoint with control points c1, c2.
   */
  void cubicTo(float c1X, float c1Y, float c2X, float c2Y, float endPointX, float endPointY) {
    QPainterPath::cubicTo(c1X, c1Y, c2X, c2Y, endPointX, endPointY);
  }

  void addRect(float x, float y, float w, float h) {
    QPainterPath::addRect(x, y, w, h);
  }

  void lineTo(float x, float y) {
    QPainterPath::lineTo(x, y);
  }
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_PATH_H_
