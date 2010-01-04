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
#include "m_device_view.h"

#include "m_device_label.h"
#include "m_slider.h"
#include "m_pad.h"

#define PARTS_HASH_SIZE 100

MDeviceView::MDeviceView(const std::string &part_id, MViewProxy *view_proxy, const std::string &name)
    : MComponent(part_id, view_proxy),
      parts_(PARTS_HASH_SIZE) {
  label_ = new MDeviceLabel(mimas_, this, T("device name"), String(name.c_str()));

  border_ = new ResizableBorderComponent(this, 0);
  addAndMakeVisible(border_);
  addAndMakeVisible(label_);
  resized();
}

//MDeviceView::~MDeviceView() {
//
//}

void MDeviceView::update(const Value &def) {
  MComponent::update(def);

  Value parts_value = def["parts"];
  if (!parts_value.is_hash()) {
    error("'parts' attribute is not a hash. Found", parts_value);
    return;
  }

  Hash *parts = parts_value.hash_;
  Hash::const_iterator it, end = parts->end();
  Value part_def;
  MComponent *part;

  for (it = parts->begin(); it != end; ++it) {
    if (parts->get(*it, &part_def) && part_def.is_hash()) {
      if (parts_.get(*it, &part)) {
        // part already exists, update
      } else {
        // create new part from definition
        Value klass = part_def["class"];
        if (!klass.is_string()) {
          error("'class' attribute missing in", part_def);
          continue;
        }

        if (klass.str() == "Slider") {
          // ================================================== Slider
          part = new MSlider(*it, view_proxy_);

        } else if (klass.str() == "Pad") {
          // ================================================== Pad
          part = new MPad(*it, view_proxy_);

        } else {
          part = NULL;
          error("Unknown class", klass);
        }
      }

      if (part) {
        part->update(part_def);
        if (!part->isVisible()) addAndMakeVisible(part);
        parts_.set(*it, part);
      }
    }
  }

}

void MDeviceView::resized() {
  // FIXME: get label width...
  border_->setBounds(
    (DEVICE_BORDER_WIDTH / 2),
    (DEVICE_BORDER_WIDTH / 2) + label_->min_height() / 2,
    getWidth() - DEVICE_BORDER_WIDTH,
    getHeight() - (label_->min_height() / 2) - DEVICE_BORDER_WIDTH
  );
  //  device_browser_->setBounds (10, 10, getWidth() - 10, getHeight() - 250);
  //  quitButton->setBounds (getWidth() - 176, getHeight() - 60, 120, 32);
  //  workspace_->setBounds (10, getHeight() - 250, getWidth() - 20, 200);
}