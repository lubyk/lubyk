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
#include "m_browser.h"
#include "m_device_list.h"
#include "mimas_window_content.h"

MBrowser::MBrowser(MimasWindowContent *mimas, const char *service_type)
    : ZeroConfBrowser(service_type),
      mimas_(mimas) {

  device_list_ = new MDeviceList(mimas_);
  addAndMakeVisible(device_list_);

  border_ = new ResizableBorderComponent(this, 0);
  addAndMakeVisible(border_);
}

void MBrowser::added_proxy(RootProxy *proxy) {
  MessageManagerLock mml;
  ScopedAutoReleasePool pool;
  device_list_->add_device(proxy);
}

void MBrowser::removed_proxy(RootProxy *proxy) {
  MessageManagerLock mml;
  ScopedAutoReleasePool pool;
  device_list_->remove_device(proxy);
}

void MBrowser::resized() {
  device_list_->setBounds(0, 0, getWidth() - LAYOUT_BROWSER_BORDER_WIDTH, getHeight() / 3.0f);
  border_->setBounds(0, 0, getWidth(), getHeight());
  mimas_->resize_except(this);
}
