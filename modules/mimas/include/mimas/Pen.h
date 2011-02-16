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
#ifndef LUBYK_INCLUDE_MIMAS_PEN_H_
#define LUBYK_INCLUDE_MIMAS_PEN_H_

#include "mimas/mimas.h"
#include "mimas/Color.h"
#include <QtGui/QPen>

#include <iostream>

namespace mimas {

/** Pen used by Painter.
 *
 */
class Pen : public QPen
{
public:
  Pen() : QPen(Qt::NoPen) {}

  Pen(float width, const Color &color, int style = Qt::SolidLine, int cap = Qt::SquareCap, int join = Qt::MiterJoin)
   : QPen(QBrush(color, Qt::SolidPattern), width, (Qt::PenStyle)style, (Qt::PenCapStyle)cap, (Qt::PenJoinStyle)join) {}

  ~Pen() {
    MIMAS_DEBUG_GC
  }

  void setWidth(float w) {
    QPen::setWidthF(w);
  }

  void setRgba(float r, float g, float b, float a=1.0) {
    QPen::setColor(QColor(r,g,b,a));
  }

  void setColor(const Color &color) {
    QPen::setColor(color);
  }

  void setHsva(float h, float s=1.0, float v=1.0, float a=1.0) {
    QColor color;
    color.setHsvF(h, s, v, a);
    QPen::setColor(color);
  }

  void setStyle(int style) {
    QPen::setStyle((Qt::PenStyle)style);
  }

  void setCapStyle(int style) {
    QPen::setCapStyle((Qt::PenCapStyle)style);
  }

  void setJoinStyle(int style) {
    QPen::setJoinStyle((Qt::PenJoinStyle)style);
  }
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_PEN_H_
