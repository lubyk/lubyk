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

#ifndef MIMAS_SRC_DEVICE_VIEW_M_DEVICE_LABEL_H_
#define MIMAS_SRC_DEVICE_VIEW_M_DEVICE_LABEL_H_
#include "m_object_proxy.h"

class MDeviceView;
class MimasWindowContent;

class MDeviceLabel : public Component {
public:
  MDeviceLabel(MimasWindowContent *mimas, MDeviceView *device_view, const String &name, const String &device_name);

  virtual void mouseDrag(const MouseEvent &e);

  virtual void mouseDown(const MouseEvent &e);

  int min_width() {
    return min_width_;
  }

  int min_height() {
    return min_height_;
  }

  virtual void paint(Graphics &g);

private:
  MimasWindowContent *mimas_;
  Label *label_;
  int min_width_;
  int min_height_;
  MDeviceView *device_view_;
  ComponentDragger dragger_;
};

#endif // MIMAS_SRC_DEVICE_VIEW_M_DEVICE_LABEL_H_