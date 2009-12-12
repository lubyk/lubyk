#ifndef MIMAS_SRC_WIDGETS_M_PAD_H_
#define MIMAS_SRC_WIDGETS_M_PAD_H_
#include "m_observable.h"

#include <iostream>

class MPad;

class MPadXRange : public MRangeWidget {
public:
  MPadXRange(MPad *pad) : pad_(pad) {}

  virtual void set_enabled(bool enabled);

  virtual void set_range(Real min, Real max);

  virtual void set_remote_value(Real value);

  virtual bool is_dragged();

private:
  MPad *pad_;
};

class MPad : public Component, public MObservable {
public:
  MPad(const String &name) : Component(name) {}

  virtual void 	mouseDrag (const MouseEvent &e) {
    std::cout << "drag: " << e.getDistanceFromDragStartX() << " x " << e.getDistanceFromDragStartY() << "\n";
  }
};

#endif // MIMAS_SRC_WIDGETS_M_PAD_H_