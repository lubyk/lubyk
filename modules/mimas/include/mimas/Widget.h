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
#ifndef RUBYK_INCLUDE_MIMAS_WIDGET_H_
#define RUBYK_INCLUDE_MIMAS_WIDGET_H_

#include "rubyk.h"
#include <QtGui/QWidget>

#include <iostream>

namespace mimas {

/** Display view.
 */
class Widget : public QWidget
{
  Q_OBJECT

public:
  Widget() {
    fill_color_.setRgbF(0.15, 0.15, 0.15);
  }

  ~Widget() {}

  QWidget *widget() {
    return this;
  }

  void resize(int w, int h) {
    QWidget::resize(w, h);
  }

  void move(int x, int y) {
    QWidget::move(x, y);
  }

  void show() {
    QWidget::show();
  }

  void activateWindow() {
    QWidget::activateWindow();
  }

  /** Set widget color.
   */
  void setHue(float hue);

protected:
  //virtual void mousePressEvent(QMouseEvent *event);
  //virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void paintEvent(QPaintEvent *event);

  /** The component's color.
   */
  float hue_;

  /** Cached border color.
   */
  QColor border_color_;

  /** Cached fill color.
   */
  QColor fill_color_;
};

} // mimas
#endif // RUBYK_INCLUDE_MIMAS_WIDGET_H_