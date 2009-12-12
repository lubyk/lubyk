#ifndef MIMAS_SRC_WIDGETS_M_SLIDER_H_
#define MIMAS_SRC_WIDGETS_M_SLIDER_H_
#include "m_observable.h"

class ObservableSlider : public Slider, public MObservable {
public:
  ObservableSlider(const String &name) : Slider(name) {}

};

#endif // MIMAS_SRC_WIDGETS_M_SLIDER_H_