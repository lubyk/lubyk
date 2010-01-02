#ifndef MIMAS_SRC_WIDGETS_M_RANGE_WIDGET_H_
#define MIMAS_SRC_WIDGETS_M_RANGE_WIDGET_H_
#include "m_observable.h"

#include <iostream>

class MObjectProxy;

class MRangeWidget : public MObservable {
public:
  MRangeWidget();

  virtual ~MRangeWidget() {}

  virtual void set_enabled(bool enabled) = 0;

  virtual void set_range(Real min, Real max) {
    min_ = min;
    max_ = max;
  }

  void set_remote_value(Real value) {
    remote_value_ = value;
    redraw();
  }

  void set_value(Real value) {
    value_ = value;
    redraw();
  }

  Real scaled_value(Real range) {
    if (max_ - min_ == 0) return 0;
    return value_ * range / (max_ - min_);
  }

  Real scaled_remote_value(Real range) {
    if (max_ - min_ == 0) return 0;
    return remote_value_ * range / (max_ - min_);
  }

  void set_scaled_value(Real position, Real range);

  void stop_drag();

  void set_proxy(MObjectProxy *proxy);

  int last_drag() {
    return last_drag_;
  }

  bool connected() {
    return proxy_ != NULL;
  }

  /* =======  Callbacks that need to be implemented in sub-classes. ========== */

  /** Should return true if the component is being dragged.
   */
  virtual bool is_dragged() = 0;

  /** Should provoque a redraw of the widget.
   */
  virtual void redraw() = 0;

protected:

  /** Time when the last dragging operation occured.
   */
  int last_drag_;

  /** Smallest possible value.
   */
  Real min_;

  /** Largest possible value.
   */
  Real max_;

  /** Current value set by user.
   */
  Real value_;

  /** Remote value.
   */
  Real remote_value_;

  /** Object proxy that is connected to this widget.
   */
  MObjectProxy *proxy_;
};

#endif // MIMAS_SRC_WIDGETS_M_RANGE_WIDGET_H_