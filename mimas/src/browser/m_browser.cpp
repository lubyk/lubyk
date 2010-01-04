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
#include "m_breadcrumbs.h"
#include "m_path_list.h"
#include "mimas_window_content.h"

MBrowser::MBrowser(MimasWindowContent *mimas, const char *service_type)
    : ZeroConfBrowser(service_type),
      mimas_(mimas) {

  border_ = new ResizableBorderComponent(this, 0);
  addAndMakeVisible(border_);

  device_list_ = new MDeviceList(this);
  addAndMakeVisible(device_list_);

  breadcrumbs_ = new MBreadcrumbs(this);
  addAndMakeVisible(breadcrumbs_);

  path_list_ = new MPathList(this);
  addAndMakeVisible(path_list_);

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
  int inner_width = getWidth() - LAYOUT_BROWSER_BORDER_WIDTH;
  int device_height = getHeight() / 3.0f;
  int breadcrumbs_height = 25;

  device_list_->setBounds(0, 0,             inner_width, device_height);
  breadcrumbs_->setBounds(0, device_height, inner_width, breadcrumbs_height);
  path_list_->setBounds(0, device_height + breadcrumbs_height, inner_width, getHeight() - device_height - breadcrumbs_height);

  border_->setBounds(0, 0, getWidth(), getHeight());
  mimas_->resize_except(this);
}

void MBrowser::select_device(RootProxy* device) {
  selected_device_ = device;
  select_container_url(Url(selected_device_->remote_location(), ""));
}

void MBrowser::select_container_url(const Url &url) {
  selected_container_url_ = url;
  std::cout << "SELECTED URL = " << url << "\n";
  breadcrumbs_->set_url(url);
  Object *object = selected_device_->object_at(url.path());
  std::cout << "FOUND OBJECT " << object << "\n";
  path_list_->set_container(object);
}





