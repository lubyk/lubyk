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

#ifndef MIMAS_SRC_DEVICE_VIEW_M_DEVICE_VIEW_H_
#define MIMAS_SRC_DEVICE_VIEW_M_DEVICE_VIEW_H_
#include "m_object_proxy.h"
#include "m_component.h"

class MDeviceLabel;
class MViewProxy;

class MDeviceView : public MComponent {
public:
  MDeviceView(const std::string &part_id, MViewProxy *view_proxy, const std::string &name);

  //virtual ~MDeviceView();

  virtual void update(const Value &def);

  virtual void mouseEnter(const MouseEvent &e) {
    hover_ = true;
    repaint();
  }

  virtual void mouseExit(const MouseEvent &e) {
    if (e.x <= 0 || e.x >= getWidth() || e.y <= 0 || e.y >= getHeight()) hover_ = false;
    repaint();
  }

  virtual void 	paint (Graphics &g);

  void resized();

private:
  /** Display the name of the device.
   */
  MDeviceLabel *label_;

  /** Contains a hash with the list of sub-components indexed by
   * part id.
   */
  THash<std::string, MComponent*> parts_;

  ResizableBorderComponent *border_;
  bool hover_;
};

#endif // MIMAS_SRC_DEVICE_VIEW_M_DEVICE_VIEW_H_