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

#ifndef MIMAS_SRC_DEVICE_BROWSER_OBJECT_PROXY_TREE_ITEM_H_
#define MIMAS_SRC_DEVICE_BROWSER_OBJECT_PROXY_TREE_ITEM_H_
#include "oscit/object_proxy.h"
#include "m_observable.h"

class MimasWindowContent;
class MObjectProxy;

class ObjectProxyTreeItem : public TreeViewItem, public MObservable {
public:

  ObjectProxyTreeItem(MimasWindowContent *mimas, MObjectProxy *proxy);

  MObjectProxy *object_proxy() {
    return object_proxy_;
  }

  virtual void paintItem(Graphics& g, int width, int height);

  virtual Component *createItemComponent();

  virtual bool mightContainSubItems();

  virtual void itemOpennessChanged(bool isNowOpen);

private:
  MimasWindowContent *mimas_;
  MObjectProxy *object_proxy_;
  bool subnodes_added_;
};

#endif // MIMAS_SRC_DEVICE_BROWSER_OBJECT_PROXY_TREE_ITEM_H_