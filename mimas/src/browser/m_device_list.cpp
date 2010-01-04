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
#include "m_device_list.h"

#include "mimas_window_content.h"
#include "m_browser.h"

MDeviceList::MDeviceList(MBrowser *browser)
  : ListBox(T("device list"), NULL),
    ListBoxModel(),
    mimas_(browser->mimas()),
    browser_(browser) {

  setModel(this);
  setMultipleSelectionEnabled(false);

  // TODO: should replace this with a callback on theme change...
  setColour(ListBox::backgroundColourId, mimas_->bg_color());
  setColour(ListBox::textColourId, mimas_->color(MTheme::BrowserLabel));
}

// ======== data management        =========== //
void MDeviceList::add_device(RootProxy *device) {
  devices_.push_back(device);
  updateContent();
}

void MDeviceList::remove_device(RootProxy *device) {
  std::vector<RootProxy*>::iterator it, end = devices_.end();
  for(it = devices_.begin(); it != end; ++it) {
    if (*it == device) {
      it = devices_.erase(it);
    }
  }
  updateContent();
}
// ======== ListBoxModel callbacks =========== //

int MDeviceList::getNumRows() {
  return devices_.size();
}

// ======== ListBox callbacks =========== //
void MDeviceList::listBoxItemClicked(int row, const MouseEvent &e) {
  browser_->select_device(devices_[row]);
}




