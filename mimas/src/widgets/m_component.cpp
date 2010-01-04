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
#include "m_component.h"

#include "mimas_window_content.h"
#include "m_view_proxy.h"

MComponent::MComponent(const std::string &part_id, MViewProxy *view_proxy)
    : part_id_(part_id), // FIXME: replace part_id by def_path...
      mimas_(view_proxy->mimas()),
      view_proxy_(view_proxy),
      root_proxy_(view_proxy->root_proxy()),
      ghost_component_(this),
      hue_(80) {}

void MComponent::update(const Value &def) {

  // ========================================== x, y, width, height

  if (def.has_key("x") || def.has_key("y") || def.has_key("width") || def.has_key("height")) {
    setBounds(
      def["x"].get_real(getX()),
      def["y"].get_real(getY()),
      def["width"].get_real(getWidth()),
      def["height"].get_real(getHeight())
    );
    if (ghost_component_.isVisible()) { // TODO:  && !is_dragged_
      ghost_component_.setVisible(false);
    }
  }

  // ========================================== hue
  if (def.has_key("hue")) {
    set_hue(def["hue"].get_real());
  }
}

void MComponent::set_hue(float hue) {
  hue_ = (hue < 0 || hue >= 360) ? 0 : hue;
  //                     hue            sat   bri   alpha
  border_color_ = Colour(hue_ / 360.0f, 1.0f, 1.0f, isEnabled() ? 1.0f : 0.3f);
  fill_color_   = Colour(hue_ / 360.0f, 0.5f, 0.5f, isEnabled() ? 1.0f : 0.3f);
}


bool MComponent::should_handle_mouse() {
  return mimas_->action_mode() && is_connected();
}

void MComponent::mouseDown(const MouseEvent &e) {
  if (!mimas_->edit_mode()) return;

  ghost_component_.setBounds(
    getX(),
    getY(),
    getWidth(),
    getHeight()
  );
  getParentComponent()->addAndMakeVisible(&ghost_component_);
  dragger_.startDraggingComponent(&ghost_component_, 0);
}

void MComponent::mouseDrag(const MouseEvent &e) {
  if (!mimas_->edit_mode()) return;

  dragger_.dragComponent(&ghost_component_, e);
}

void MComponent::mouseUp(const MouseEvent &e) {
  // ghost dragging ended
  Value view;
  Value parts;
  Value def;
  def.set("x",ghost_component_.getX());
  def.set("y",ghost_component_.getY());
  def.set("width",ghost_component_.getWidth());
  def.set("height",ghost_component_.getHeight());
  parts.set(part_id_, def);
  view.set("parts", parts);
  view_proxy_->update_remote(view);
}


// ================== on_registration_callback

class MComponent::OnRegistrationCallback : public TCallback<MComponent, &MComponent::on_registration_callback> {
public:
  OnRegistrationCallback(MComponent *observer, const Value &def)
      : TCallback<MComponent, &MComponent::on_registration_callback>(observer, new Value(def.to_json())) {}

  virtual ~OnRegistrationCallback() {
    Value *def = (Value*)data_;
    delete def;
  }
};

void MComponent::add_callback(const std::string &path, const Value &def) {
  std::cout << "Add callback '" << path << "' with " << def << "\n";
  root_proxy_->adopt_callback_on_register(path,
    new MComponent::OnRegistrationCallback(this, def)
  );
}

void MComponent::on_registration_callback(void *data) {
  Value *def = (Value*)data;
  MessageManagerLock mml;
  std::cout << "Callback triggered with " << *def << "\n";
  update(*def);
}

