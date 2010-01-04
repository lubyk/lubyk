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
#include "m_slider.h"

#include "m_object_proxy.h"
#include "mimas_window_content.h"

void MSlider::update(const Value &def) {
  Value err;

  // ========================================== x, y, width, height
  MComponent::update(def);
  if (getWidth() > getHeight()) {
    set_slider_type(MSlider::HorizontalSliderType);
  } else {
    set_slider_type(MSlider::VerticalSliderType);
  }

  // ========================================== connect

  if (!proxy_ && def.has_key("connect")) {
  // TODO: implement disconnect so that we can change connection...
    const Value connect_path = def["connect"];
    if (!connect_path.is_string()) {
      error("invalid 'connect' attribute in", def);
    } else {
      MObjectProxy *proxy = TYPE_CAST(MObjectProxy, root_proxy_->find_or_build_object_at(connect_path.str(), &err));
      if (!proxy) {
        error("could not connect", err);
        // this should never happen since find_or_build_object_at should build dummy
        // objects that will receive a "type_changed" when they are ready...
        add_callback(connect_path.str(), def);
      } else {
        proxy->connect(this);
      }
    }
  }
}


bool MSlider::is_connected() {
  return proxy_ != NULL && proxy_->is_connected();
}

void MSlider::mouseDown(const MouseEvent &e) {
  if (should_handle_mouse()) {
    is_dragged_ = true;
    mouseDrag(e);
  } else {
    MComponent::mouseDown(e);
  }
}

void MSlider::mouseDrag(const MouseEvent &e) {
  if (should_handle_mouse()) {
    if (slider_type_ == HorizontalSliderType) {
      set_scaled_value(e.x, getWidth());
    } else {
      set_scaled_value(getHeight() - e.y, getHeight());
    }
    proxy_->set_value(Value(value_));
  } else {
    MComponent::mouseDrag(e);
  }
}

void MSlider::mouseUp(const MouseEvent &e) {
  if (is_dragged_) {
    is_dragged_ = false;
    last_drag_ = proxy_->time_ref().elapsed();
  } else {
    MComponent::mouseUp(e);
  }
}
