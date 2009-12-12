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

void MPadRange::set_scaled_value(Real position, Real range) {
  if (!range) return;
  Real r = position * (max_ - min_) / range;
  if (r < min_) r = min_;
  if (r > max_) r = max_;
  value_.r = r;
  proxy_->set_value(value_);
}

void MPadRange::stop_drag() {
  last_drag_ = proxy_->time_ref().elapsed();
}

/** ================ Pad ============ */
