#include "mimas.h"
#include "m_pad.h"

#include "m_object_proxy.h"
/** ================ X Range ============ */

void MPadRange::set_enabled(bool enabled) {
  pad_->set_enabled(enabled);
}

bool MPadRange::is_dragged() {
  return pad_->is_dragged();
}

void MPadRange::redraw() {
  pad_->repaint();
}

/** ================ Pad ============ */

void MPad::set_enabled(bool enabled) {
  setEnabled(enabled);
  set_hue(hue_);
  // repaint(); ?
}

void MPad::update(const Value &def) {
  Value err;

  // ========================================== x, y, width, height
  MComponent::update(def);

  // ========================================== connect_x

  if (def.has_key("connect_x") && !range_x_.proxy_) {
    // TODO: implement disconnect so that we can change connection...

    Value connect_path = def["connect_x"];
    if (!connect_path.is_string()) {
      error("invalid 'connect_x' attribute in", def);
    } else {
      MObjectProxy *proxy = TYPE_CAST(MObjectProxy, root_proxy_->find_or_build_object_at(connect_path.str(), &err));
      if (!proxy) {
        error("could not connect x", err);
        // this should never happen since find_or_build_object_at should build dummy
        // objects that will receive a "type_changed" when they are ready...
        add_callback(connect_path.str(), def);
      } else {
        proxy->connect(&range_x_);
        set_hue(hue_); // update color
      }
    }
  }

  // ========================================== connect_y

  if (def.has_key("connect_y") && !range_y_.proxy_) {
    // TODO: implement disconnect so that we can change connection...

    Value connect_path = def["connect_y"];
    if (!connect_path.is_string()) {
      error("invalid 'connect_y' attribute in", def);
    } else {
      MObjectProxy *proxy = TYPE_CAST(MObjectProxy, root_proxy_->find_or_build_object_at(connect_path.str(), &err));
      if (!proxy) {
        error("could not connect y", err);
        // this should never happen since find_or_build_object_at should build dummy
        // objects that will receive a "type_changed" when they are ready...
        add_callback(connect_path.str(), def);
      } else {
        proxy->connect(&range_y_);
      }
    }
  }

  // ========================================== hue

  if (def.has_key("hue")) {
    set_hue(def["hue"].get_real());
  }
}


bool MPad::is_connected() {
  return range_x_.proxy_ != NULL && range_x_.proxy_->is_connected() &&
         range_y_.proxy_ != NULL && range_y_.proxy_->is_connected();
}

void MPad::mouseDown(const MouseEvent &e) {
  dragged_ = true;
  if (!is_connected()) return;
  range_x_.set_scaled_value(e.x, getWidth());
  range_y_.set_scaled_value(getHeight() - e.y, getHeight());
  repaint();
}

void MPad::mouseDrag(const MouseEvent &e) {
  if (!is_connected()) return;
  range_x_.set_scaled_value(e.x, getWidth());
  range_y_.set_scaled_value(getHeight() - e.y, getHeight());
  repaint();
}

void MPad::mouseUp(const MouseEvent &e) {
  dragged_ = false;
  if (!is_connected()) return;
  range_x_.stop_drag();
  range_y_.stop_drag();
}
