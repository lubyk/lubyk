#ifndef MIMAS_SRC_WIDGETS_M_RANGE_WIDGET_H_
#define MIMAS_SRC_WIDGETS_M_RANGE_WIDGET_H_
#include "m_observable.h"

#include <iostream>

class MObjectProxy;

class MRangeWidget : public MObservable {
public:
  MRangeWidget() : last_drag_(0), proxy_(NULL) {}

  virtual ~MRangeWidget() {}

  virtual void set_enabled(bool enabled) = 0;

  virtual void set_range(Real min, Real max) = 0;

  virtual void set_remote_value(Real value) = 0;

  virtual void handle_value_change(const Value &value) = 0;

  virtual bool is_dragged() = 0;

  virtual void redraw() = 0;

  void set_proxy(MObjectProxy *proxy);

  int last_drag() {
    return last_drag_;
  }

  void set_last_drag(int last_drag) {
    last_drag_ = last_drag;
  }

protected:
  int last_drag_;
  MObjectProxy *proxy_;
};

#endif // MIMAS_SRC_WIDGETS_M_RANGE_WIDGET_H_