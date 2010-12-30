/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
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
#ifndef RUBYK_INCLUDE_MIMAS_SLIDER_H_
#define RUBYK_INCLUDE_MIMAS_SLIDER_H_

#include "rubyk.h"

#include <QtGui/QSlider>

#include <iostream>

using namespace rubyk;

namespace mimas {

/** Slider (async slider).
 *
 * @dub lib_name:'Slider_core'
 */
class Slider : public QWidget
{
  Q_OBJECT
  Q_PROPERTY(double value_ READ value WRITE setValue)
  Q_PROPERTY(QColor color_ READ color WRITE setColor)
  
  Worker *worker_;
  double value_;
  QColor color_;
public:
  Slider(rubyk::Worker *worker, int orientation = (int)Qt::Horizontal, QWidget *parent = 0)
   : QSlider((Qt::Orientation)orientation, parent),
     worker_(worker) {}

  ~Slider() {}
  
  QColor color() const {
    return color_;
  }
  
  void setColor(const QColor &color) {
    color_ = color;
  }
  
  double value() const {
    return value_;
  }
  
  void setValue(double value) {
    // we unlock in case the 'setValue' triggers a Callback
    // FIXME: this looks like a HACK: what happens if the unlock code
    // gets fired through a gui --> setValue ? We should move this into
    // the bindings...
    ScopedUnlock  unlock(worker_);
    value_ = value;
  }

  QWidget *widget() {
    return this;
  }

  QObject *object() {
    return this;
  }
protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void paintEvent(QPaintEvent *event);
};

} // mimas
#endif // RUBYK_INCLUDE_MIMAS_SLIDER_H_