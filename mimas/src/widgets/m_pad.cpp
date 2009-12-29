#include "mimas.h"
#include "m_pad.h"

#include "m_object_proxy.h"
/** ================ X Range ============ */

void MPadRange::set_enabled(bool enabled) {
  pad_->setEnabled(enabled);
}

bool MPadRange::is_dragged() {
  return pad_->is_dragged();
}

void MPadRange::redraw() {
  pad_->repaint();
}

/** ================ Pad ============ */

void MPad::mouseDown(const MouseEvent &e) {
  dragged_ = true;
  range_x_.set_scaled_value(e.x, getWidth());
  range_y_.set_scaled_value(getHeight() - e.y, getHeight());
  repaint();
}

void MPad::mouseDrag(const MouseEvent &e) {
  range_x_.set_scaled_value(e.x, getWidth());
  range_y_.set_scaled_value(getHeight() - e.y, getHeight());
  repaint();
}

void MPad::mouseUp(const MouseEvent &e) {
  dragged_ = false;
  range_x_.stop_drag();
  range_y_.stop_drag();
}
