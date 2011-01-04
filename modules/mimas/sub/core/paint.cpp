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

#include "mimas/Widget.h"
#include "mimas/Slider.h"

#include <QPainter>

#define SLIDER_BORDER_WIDTH 2
#define WIDGET_THUMB_SIZE 16

namespace mimas {

// =============================================
// ==             Widget                      ==
// =============================================

void Widget::paintEvent(QPaintEvent *event) {
  QPainter p(this);
  p.fillRect(rect(), fill_color_);
  QWidget::paintEvent(event);
}

// =============================================
// ==             Label                       ==
// =============================================
// No custom draw for the moment
//void Label::paintEvent(QPaintEvent *event) {
//  QPainter p(this);
//
//}

// =============================================
// ==             Slider                      ==
// =============================================

void Slider::paintEvent(QPaintEvent *event) {
  QPainter p(this);

  // if (connected()) {
  //   g.fillAll(mimas_->bg_color());
  // } else {
  //   g.fillAll(mimas_->color(MTheme::Disconnected));
  // }

  if (slider_type_ == VerticalSliderType) {
    // vertical slider
    int remote_pos = range_.remotePosition(height());
    int local_pos  = range_.position(height());

    // filled slider value
    p.fillRect(
      0,
      height() - remote_pos,
      width(),
      remote_pos,
      fill_color_
    );

    // line on top of value
    // g.setColour(border_color_);
    //g.drawLine(
    //  SLIDER_BORDER_WIDTH / 2,
    //  h - SLIDER_BORDER_WIDTH / 2,
    //  getWidth() - SLIDER_BORDER_WIDTH,
    //  h - SLIDER_BORDER_WIDTH / 2,
    //  SLIDER_BORDER_WIDTH
    //);

    if (range_.isDragged() || remote_pos != local_pos) {
      // thumb
      //g.setColour(mimas_->color(MTheme::WidgetThumbBG));
      p.fillRect(
        0,
        height() - local_pos - WIDGET_THUMB_SIZE / 2,
        width(),
        WIDGET_THUMB_SIZE,
        QColor(255, 255, 255, 32)
      );
    }

  } else {
    // horizontal slider
    int remote_pos = range_.remotePosition(width());
    int local_pos  = range_.position(width());

    // filled slider value
    p.fillRect(
      0,
      0,
      remote_pos,
      height(),
      fill_color_
    );

    if (range_.isDragged() || remote_pos != local_pos) {
      // thumb
      //g.setColour(mimas_->color(MTheme::WidgetThumbBG));
      p.fillRect(
        local_pos - WIDGET_THUMB_SIZE / 2,
        0,
        WIDGET_THUMB_SIZE,
        height(),
        QColor(255, 255, 255, 32)
      );
    }
  }

  p.setPen(QPen(border_color_, SLIDER_BORDER_WIDTH));
  p.setBrush(Qt::NoBrush);
  p.drawRect(
    SLIDER_BORDER_WIDTH / 2,
    SLIDER_BORDER_WIDTH / 2,
    width() -  SLIDER_BORDER_WIDTH,
    height() - SLIDER_BORDER_WIDTH
  );
}


} // mimas