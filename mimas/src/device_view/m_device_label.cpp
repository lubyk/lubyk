/*
  ==============================================================================

   This file is part of the MIMAS project (http://rubyk.org/mimas)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

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

#include "mimas.h"
#include "m_device_label.h"

#include "m_device_view.h"
#include "mimas_window_content.h"

#define LABEL_MARGIN_T 0

MDeviceLabel::MDeviceLabel(MimasWindowContent *mimas, MDeviceView *device_view, const String &name, const String &device_name)
    : mimas_(mimas),
      device_view_(device_view) {
  label_ = new Label(name, device_name);
  label_->setFont(Font(15.0f, Font::bold));
  label_->setInterceptsMouseClicks(false, false);
  min_width_  = label_->getFont().getStringWidth(label_->getText());
  min_height_ = label_->getFont().getHeight();

  label_->setBounds(DEVICE_ROUNDED_RADIUS * 2.0, LABEL_MARGIN_T, min_width_, min_height_);
  setBounds(0, 0, min_width_ + 2 * DEVICE_ROUNDED_RADIUS * 2.0, min_height_ + 2 * LABEL_MARGIN_T);

  addAndMakeVisible(label_);

  setInterceptsMouseClicks(true, true);
}

void MDeviceLabel::mouseDown(const MouseEvent &e) {
  dragger_.startDraggingComponent(device_view_, 0);
}

void MDeviceLabel::mouseDrag(const MouseEvent &e) {
  dragger_.dragComponent(device_view_, e);
}


