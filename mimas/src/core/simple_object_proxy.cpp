#include "mimas.h"
#include "simple_object_proxy.h"


ObservableSlider *SimpleObjectProxy::build_slider() {
  ObservableSlider *slider = new ObservableSlider(String(name_.c_str()));
  slider->setRange(type()[1].r, type()[2].r);
  slider->setValue(value_.r);
  observe(slider);
  return slider;
}