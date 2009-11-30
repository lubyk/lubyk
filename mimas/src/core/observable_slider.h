#ifndef MIMAS_SRC_CORE_OBSERVABLE_SLIDER_H_
#define MIMAS_SRC_CORE_OBSERVABLE_SLIDER_H_
#include "oscit/observable.h"

class ObservableSlider : public Slider, public Observable {
public:
  ObservableSlider(const String &name) : Slider(name) {}
};

#endif // MIMAS_SRC_CORE_OBSERVABLE_SLIDER_H_