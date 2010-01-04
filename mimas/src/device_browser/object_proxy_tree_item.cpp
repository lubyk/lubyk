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
#include "mimas_window_content.h"
#include "object_proxy_tree_item.h"
#include "m_object_proxy.h"
#include "m_slider.h"

ObjectProxyTreeItem::ObjectProxyTreeItem(MimasWindowContent *mimas, MObjectProxy *object)
    : mimas_(mimas),
      subnodes_added_(false) {
  set_and_hold(&object_proxy_, object);
}

void ObjectProxyTreeItem::paintItem(Graphics& g, int width, int height) {
    // if this item is selected, fill it with a background colour..
  if (isSelected()) g.fillAll(Colours::blue.withAlpha (0.3f));
  // use a "colour" attribute in the xml tag for this node to set the text colour..
  g.setColour (Colours::black);

  g.setFont (height * 0.7f);

  // draw the xml element's tag name..
  g.drawText(String(object_proxy_->name().c_str()),
    4, 0, width - 4, height,
    Justification::centredLeft, true);
}

bool ObjectProxyTreeItem::mightContainSubItems() {
  return object_proxy_->children().size() > 0;
}

Component *ObjectProxyTreeItem::createItemComponent() {
  Value type = object_proxy_->type();
  if (type.type_id() == H("fffss")) {
    MSlider *slider = new MSlider(mimas_, object_proxy_->url().c_str());
    object_proxy_->connect(slider);
    slider->setBounds(100, 2, 250, 20);
    slider->set_slider_type(MSlider::HorizontalSliderType);
    return slider;
  } else {
    return 0; // use paintItem instead
  }
}

void ObjectProxyTreeItem::itemOpennessChanged(bool isNowOpen) {
  if (isNowOpen && !subnodes_added_) {
    // FIXME: make sure we do not accept changes to this tree now !
    const THash<std::string, Object *> children = object_proxy_->children();
    std::list<std::string>::const_iterator it = children.begin();
    std::list<std::string>::const_iterator end = children.end();
    for (; it != end; ++it) {
      Object *object;
      assert(children.get(*it, &object));
      MObjectProxy *object_proxy = TYPE_CAST(MObjectProxy, object);
      assert(object_proxy);
      object_proxy->sync_children();
      addSubItem(object_proxy->object_proxy_view());
    }
    subnodes_added_ = true;
  }
}