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

#include "mimas/RangeWidget.h"
#include "mimas/Widget.h"

#include <QtGui/QSlider>

#include <iostream>

using namespace rubyk;

namespace mimas {

/** Slider (async slider).
 *
 * @dub lib_name:'Slider_core'
 */
class Slider : public Widget
{
  Q_OBJECT

public:

  enum Defaults {
    DefaultWidth  = 30,
    DefaultHeight = 150
  };

  enum SliderType {
    HorizontalSliderType = 1,
    VerticalSliderType = 2,
  };


  Slider(int type = (int)VerticalSliderType, QWidget *parent = 0)
   : slider_type_((SliderType)type),
     range_(this) {}

  ~Slider() {}

  QWidget *widget() {
    return this;
  }

  QObject *object() {
    return this;
  }

  void setHue(int hue) {
    Widget::setHue(hue);
  }

  virtual QSize sizeHint() const {
    if (slider_type_ == VerticalSliderType) {
      return QSize(DefaultWidth, DefaultHeight);
    } else {
      return QSize(DefaultHeight, DefaultWidth);
    }
  }

public slots:
  /** Update slider when remote changes.
   * This is called by zmq when it receives a value change notification.
   */
  void setValue(double remote_value) {
    if (range_.setValue(remote_value)) {
      update();
    }
  }

signals:

  /** Emit when the user drags and changes the value.
   */
  void valueChanged(double);

protected:
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent* event);
  virtual void paintEvent(QPaintEvent *event);

private:
  SliderType slider_type_;
  RangeWidget range_;
};

} // mimas
#endif // RUBYK_INCLUDE_MIMAS_SLIDER_H_