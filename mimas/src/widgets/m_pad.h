#ifndef MIMAS_SRC_WIDGETS_M_PAD_H_
#define MIMAS_SRC_WIDGETS_M_PAD_H_
#include "m_observable.h"

#include <iostream>

#include "m_range_widget.h"

class MPad;

class MPadRange : public MRangeWidget {
public:
  MPadRange(MPad *pad)
      : pad_(pad),
        min_(0),
        max_(1),
        remote_value_(0),
        value_(0) {}

  virtual ~MPadRange() {}

  virtual void set_enabled(bool enabled);

  virtual void set_range(Real min, Real max) {
    min_ = min;
    max_ = max;
  }

  virtual void set_remote_value(Real value) {
    remote_value_ = value;
  }

  virtual void handle_value_change(const Value &value) {
    value_ = value;
    redraw();
  }

  virtual bool is_dragged();

  virtual void redraw();

  void set_scaled_value(Real position, Real range);

  Real scaled_value(Real range) {
    if (max_ - min_ == 0) return 0;
    return value_.r * range / (max_ - min_);
  }

private:
  friend class MPad;
  MPad *pad_;
  Real min_;
  Real max_;
  Real remote_value_;
  Value value_;
  Real position_;
};


class MPad : public Component, public MObservable {
public:
  MPad(const String &name)
      : Component(name),
        range_x_(this),
        range_y_(this),
        dragged_(false) {}

  virtual bool is_dragged() {
    return dragged_;
  }

  /** ======== Component callbacks ======== */

  virtual void mouseDrag (const MouseEvent &e) {
    range_x_.set_scaled_value(e.x, getWidth());
    range_y_.set_scaled_value(getHeight() - e.y, getHeight());
  }

  virtual void mouseUp(const MouseEvent &e) {
    dragged_ = false;
  }

  MRangeWidget *range_x() {
    return &range_x_;
  }

  MRangeWidget *range_y() {
    return &range_y_;
  }

  virtual void paint(Graphics& g) {
    float radius = 8;
    float pos_x = range_x_.scaled_value(getWidth())  - radius;
    float pos_y = getHeight() - range_y_.scaled_value(getHeight()) - radius;

    g.fillAll(Colours::grey);

    g.setColour(Colours::white);
    g.fillEllipse(pos_x, pos_y, 2*radius, 2*radius);

    g.setColour(Colours::black);
    g.drawEllipse(pos_x, pos_y, 2*radius, 2*radius, 2.0f);
  }

private:
  bool dragged_;
  MPadRange range_x_;
  MPadRange range_y_;
};

#endif // MIMAS_SRC_WIDGETS_M_PAD_H_