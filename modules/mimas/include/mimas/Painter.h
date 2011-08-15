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
#ifndef LUBYK_INCLUDE_MIMAS_PAINTER_H_
#define LUBYK_INCLUDE_MIMAS_PAINTER_H_

#include "mimas/mimas.h"
#include "mimas/Color.h"
#include "mimas/Path.h"
#include "mimas/Pen.h"
#include "mimas/Brush.h"

#include <QtGui/QPainter>
//#include <QtGui/QPainter>

#include <iostream>

namespace mimas {

/** Painter.
 *
 * @dub destructor: 'dub_destroy'
 */
class Painter : public QPainter, public DeletableOutOfLua
{
public:
  Painter(QWidget *widget) :
   QPainter(widget) {
    setRenderHints(QPainter::Antialiasing);
  }

  ~Painter() {
    MIMAS_DEBUG_GC
  }

  void fillRect( int x, int y, int width, int height, const Color &color) {
    QPainter::fillRect(x, y, width, height, color);
  }

  void drawPath(const Path &path) {
    QPainter::drawPath(path);
  }

  void setPen(const Pen &pen) {
    QPainter::setPen(pen);
  }

  void setPen(float width, const Color &color) {
    QPainter::setPen(Pen(width, color));
  }

  void setPen(float width, float h, float s=1.0, float v=1.0, float a=1.0) {
    QPainter::setPen(Pen(width, h, s, v, a));
  }

  void setBrush(const Brush &brush) {
    QPainter::setBrush(brush);
  }

  void setBrush(const Color &color) {
    // if we do not wrap, it seems that the brush is changed
    // BUG:
    // p:setBrush(mimas.NoBrush)
    // p:setBrush(mimas.colors.Red)
    // ...
    // p:setBrush(mimas.NoBrush) --- creates red surfaces
    QPainter::setBrush(QBrush(color));
  }

  /** Set brush with the given hsva color.
   */
  void setBrush(float h, float s=1.0, float v=1.0, float a=1.0) {
    QPainter::setBrush(Brush(h, s, v, a));
  }

  /** Draw a single point with the current pen.
   */
  void drawPoint(int x, int y) {
    QPainter::drawPoint(x, y);
  }

  /** Draw a single line with the current pen.
   */
  void drawLine(int x1, int y1, int x2, int y2) {
    QPainter::drawLine(x1, y1, x2, y2);
  }

  /** Draw a chord (filled arc). TODO: How the rectangle is used is not clear...
   * Angles are specified in degrees.
   */
  void drawChord(int x, int y, int width, int height, float startAngle, float spanAngle) {
    QPainter::drawChord(x, y, width, height, startAngle * 16, spanAngle * 16);
  }

  /** Draw a rectangle.
   */
  void drawRect(float x, float y, float w, float h) {
    QPainter::drawRect(QRectF(x, y, w, h));
  }

  /** Default value for yRadius = xRadius.
   */
  void drawRoundedRect(float x, float y, float w, float h, float xRadius, lua_State *L) {
    float yRadius = lua_gettop(L) > 6 ? luaL_checknumber(L, 7) : xRadius;
    QPainter::drawRoundedRect(QRectF(x, y, w, h), xRadius, yRadius);
  }

  /** Draw an ellipse in the given rectangle.
   */
  void drawEllipse(int x, int y, int width, int height) {
    QPainter::drawEllipse(x, y, width, height);
  }

  /** Draw some text.
   */
  void drawText(float x, float y, float w, float h, int flags, const char *text) {
    QPainter::drawText(QRectF(x, y, w, h), flags, QString(text));
  }
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_PAINTER_H_
