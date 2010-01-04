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

#ifndef MIMAS_SRC_DEVICE_BROWSER_DEVICE_BROWSER_H_
#define MIMAS_SRC_DEVICE_BROWSER_DEVICE_BROWSER_H_
#include "oscit/zeroconf_browser.h"
#include "device_browser_root.h"

class MimasWindowContent;
class MDeviceList;
class MBreadcrumbs;

class MBrowser : public Component, public ZeroConfBrowser {
public:
  MBrowser(MimasWindowContent *mimas, const char *service_type);

  virtual void added_proxy(RootProxy *proxy);

  virtual void removed_proxy(RootProxy *proxy);

  virtual void paint(Graphics &g);

  virtual void resized();

private:
  MimasWindowContent *mimas_;

  /** List of found devices.
   */
  MDeviceList *device_list_;

  /** Currently selected device.
   */
  //RootProxy *selected_device_;

  /** Currently selected parent path (this is the
   * parent of the items listed in the children list).
   */
  //MObjectProxy *selected_parent_;

  /** Path hierarchy of the currently selected parent path.
   */
  //MBreadcrumbs *breadcrumbs_;

  /** List of sub-paths in the currently selected path (selected_parent_).
   */
  //MPathList *children_list_;

  /** Right border to resize the width of the device browser.
   */
  ResizableBorderComponent *border_;
};

#endif // MIMAS_SRC_DEVICE_BROWSER_DEVICE_BROWSER_H_