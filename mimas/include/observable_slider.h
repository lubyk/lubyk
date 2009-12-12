#ifndef MIMAS_SRC_CORE_OBSERVABLE_SLIDER_H_
#define MIMAS_SRC_CORE_OBSERVABLE_SLIDER_H_
#include "observable_in_juce.h"

class ObservableSlider : public Slider, public ObservableInJuce {
public:
  ObservableSlider(const String &name) : Slider(name) {}

};

#endif // MIMAS_SRC_CORE_OBSERVABLE_SLIDER_H_