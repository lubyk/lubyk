#include "mimas.h"
#include "m_slider.h"

#include "m_object_proxy.h"


void MSlider::mouseDown(const MouseEvent &e) {
  is_dragged_ = true;
  mouseDrag(e);
}

void MSlider::mouseDrag(const MouseEvent &e) {
  if (slider_type_ == HorizontalSliderType) {
    set_scaled_value(e.x, getWidth());
  } else {
    set_scaled_value(getHeight() - e.y, getHeight());
  }

  proxy_->set_value(Value(value_));
}

void MSlider::mouseUp(const MouseEvent &e) {
  is_dragged_ = false;
  last_drag_ = proxy_->time_ref().elapsed();
}
