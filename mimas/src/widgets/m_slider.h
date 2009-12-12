#ifndef MIMAS_SRC_WIDGETS_M_SLIDER_H_
#define MIMAS_SRC_WIDGETS_M_SLIDER_H_
#include "m_observable.h"

class MSlider : public Slider, public MRangeWidget {
public:
  MSlider(const String &name) : Slider(name) {}

  /** =========== MRangeWidget callbacks ========== */

  virtual void set_enabled(bool enabled) {
    setEnabled(enabled);
  }

  virtual void set_range(Real min, Real max) {
    setRange(min, max);
  }

  virtual void set_remote_value(Real value) {
    setComponentProperty(T("RemoteValue"), value);
    redraw();
  }

  virtual void handle_value_change(const Value &value) {
    setValue(value.r, false);
  }

  virtual void redraw() {
    repaint();
  }

  virtual bool is_dragged() {
    return getThumbBeingDragged() != -1;
  }

  /** =========== Slider callbacks       ========== */

  virtual void valueChanged() {
    // user slider being dragged
    proxy_->set_value(Value(getValue()));
  }

  virtual void stoppedDragging() {
    last_drag_ = proxy_->time_ref().elapsed();
  }
};

#endif // MIMAS_SRC_WIDGETS_M_SLIDER_H_