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
#include "mimas/Label.h"
#include "mimas/Slider.h"

#include <QPainter>

#define SLIDER_BORDER_WIDTH 2
#define WIDGET_THUMB_SIZE 14

namespace mimas {

// =============================================
// ==             Widget                      ==
// =============================================

void Widget::paintEvent(QPaintEvent *event) {
  QPainter p(this);
  if (!parent()) {
    // window
    p.fillRect(rect(), palette().color(QPalette::Window));
  }
  QWidget::paintEvent(event);
}

// =============================================
// ==             Label                       ==
// =============================================
void Label::paintEvent(QPaintEvent *event) {
  if (hue_ != -1) {
    int hue = (hue_ < 0 || hue_ >= 360) ? 0 : hue_;
    setStyleSheet(QString(".%1 { color:hsv(%2, 255, 255) }").arg(cssClass()).arg(hue));
  }
  QLabel::paintEvent(event);
}

// =============================================
// ==             Slider                      ==
// =============================================

void Slider::paintEvent(QPaintEvent *event) {
  QPainter p(this);
  QPalette palette = this->palette();
  int w = width();
  int h = height();

  if (hue_ != -1) {
    mimas::setHue(palette, hue_);
  }

  // color = slider value (WindowText)
  // background = background (Window)
  // alternate-background-color = border color (AlternateBase)
  // selection-background-color: not used (Highlighted)
  // selection-color: slider button (HighlightedText)

  // background
  p.fillRect(
    0,
    0,
    w,
    h,
    palette.color(QPalette::Window)
  );

  if (slider_type_ == VerticalSliderType) {
    // vertical slider
    int remote_pos = range_.remotePosition(h);
    int local_pos  = range_.position(h);

    // filled slider value
    p.fillRect(
      0,
      h - remote_pos,
      w,
      remote_pos,
      palette.color(QPalette::WindowText)
    );

    // line on top of value
    // g.setColour(border_color_);
    //g.drawLine(
    //  border_width / 2,
    //  h - border_width / 2,
    //  getWidth() - border_width,
    //  h - border_width / 2,
    //  border_width
    //);

    if (range_.isDragged() || remote_pos != local_pos) {
      // thumb
      p.fillRect(
        0,
        h - local_pos - WIDGET_THUMB_SIZE / 2,
        w,
        WIDGET_THUMB_SIZE,
        palette.color(QPalette::HighlightedText)
      );
    }

  } else {
    // horizontal slider
    int remote_pos = range_.remotePosition(w);
    int local_pos  = range_.position(w);

    // filled slider value
    p.fillRect(
      0,
      0,
      remote_pos,
      h,
      palette.color(QPalette::WindowText)
    );

    if (range_.isDragged() || remote_pos != local_pos) {
      // thumb
      //g.setColour(mimas_->color(MTheme::WidgetThumbBG));
      p.fillRect(
        local_pos - WIDGET_THUMB_SIZE / 2,
        0,
        WIDGET_THUMB_SIZE,
        h,
        palette.color(QPalette::HighlightedText)
      );
    }
  }

  QPen pen(palette.color(QPalette::AlternateBase), border_width_);
  pen.setJoinStyle(Qt::MiterJoin);
  p.setPen(pen);
  p.setBrush(Qt::NoBrush);
  p.drawRect(
    border_width_ / 2,
    border_width_ / 2,
    w -  border_width_,
    h - border_width_
  );
}


} // mimas