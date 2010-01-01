#include "mimas.h"
#include "m_slider.h"

#include "m_object_proxy.h"
#include "mimas_window_content.h"

void MSlider::mouseDown(const MouseEvent &e) {
  if (mimas_->action_mode()) {
    is_dragged_ = true;
    mouseDrag(e);
  } else if (mimas_->edit_mode()) {
    dragger_.startDraggingComponent(this, 0);
  }
}

void MSlider::mouseDrag(const MouseEvent &e) {
  if (mimas_->action_mode()) {
    if (slider_type_ == HorizontalSliderType) {
      set_scaled_value(e.x, getWidth());
    } else {
      set_scaled_value(getHeight() - e.y, getHeight());
    }
    proxy_->set_value(Value(value_));

  } else if (mimas_->edit_mode()) {
    dragger_.dragComponent(this, e);
  }
}

void MSlider::mouseUp(const MouseEvent &e) {
  is_dragged_ = false;
  last_drag_ = proxy_->time_ref().elapsed();
}
