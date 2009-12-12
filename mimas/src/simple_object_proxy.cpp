#include "mimas.h"
#include "simple_object_proxy.h"

#include <iostream>

ObservableSlider *SimpleObjectProxy::build_slider() {
  ObservableSlider *slider = new ObservableSlider(String(name_.c_str()));
  if (is_connected()) {
    slider->setRange(type()[1].r, type()[2].r);
  } else {
    slider->setEnabled(false);
  }
  observe(slider);
  return slider;
}

void SimpleObjectProxy::type_changed() {
  if (type().type_id() == H("fffss")) {
    MessageManagerLock mml;
    // RangeIO
    std::list<ObservableSlider*>::iterator it, end = sliders_.end();
    for (it = sliders_.begin(); it != end; ++it) {
      std::cout << *it << "\n";
      (*it)->setEnabled(true);
      (*it)->setRange(type()[1].r, type()[2].r);
    }
  }
}