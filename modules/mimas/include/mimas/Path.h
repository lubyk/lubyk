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
 * @dub super: 'QPainterPath'
 */
class Path : public QPainterPath
{
  QPainterPathStroker *stroker_;
public:
  Path() : stroker_(NULL) {}

  Path(const QPainterPath &path)
   : QPainterPath(path),
     stroker_(NULL) {
    MIMAS_DEBUG_GX("Path")
  }

  ~Path() {
    MIMAS_DEBUG_GX("~Path")
    if (stroker_) {
      delete stroker_;
    }
  }

  /** Return a new path that corresponds to the outline of the current
   * path (takes a curve, returns a surface). This new path can be used
   * to check intersection for example.
   */
  Path outlineForWidth(float width) {
    if (!stroker_) {
      stroker_ = new QPainterPathStroker();
    }
    stroker_->setWidth(width);
    return Path(stroker_->createStroke(*this));
  }

  /** Return true if the *shape* contains the given point.
   * This is a test against a surface. To test intersection with a
   * line, use 'outlineForWidth' first.
   */
  bool contains(float x, float y) {
    return QPainterPath::contains(QPointF(x, y));
  }
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_PATH_H_
