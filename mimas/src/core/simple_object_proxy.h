#ifndef MIMAS_SRC_CORE_SIMPLE_OBJECT_PROXY_H_
#define MIMAS_SRC_CORE_SIMPLE_OBJECT_PROXY_H_
#include "oscit/object_proxy.h"
#include "object_proxy_view.h"
#include "observable_slider.h"

class SimpleObjectProxy : public ObjectProxy, public SliderListener {
public:
  SimpleObjectProxy(const std::string &name, const Value &type) : ObjectProxy(name, type) {
    set_and_hold(&tree_view_item_, new ObjectProxyView(this));
  }

  ObjectProxyView *object_proxy_view() const { // FIXME: how to keep constness ok here ?
    return tree_view_item_;
  }

  void observe(ObservableSlider *slider) {
    append_and_hold(&sliders_, slider);
    slider->addListener(this);
  }

  virtual void sliderValueChanged (Slider *slider) {
    // user slider being dragged
    set_value(Value(slider->getValue()));
  }

  virtual void 	sliderDragEnded (Slider *slider) {
    // sync user slider with real value
    value_changed();
  }

  virtual void value_changed() {
    // update real value slider
    MessageManagerLock mml;
    if (value_.is_real()) {
      std::list<ObservableSlider*>::iterator it, end = sliders_.end();
      for (it = sliders_.begin(); it != end; ++it)
        (*it)->setValue(value_.r, false); // do not notify
    }
  }

private:
  std::list<ObservableSlider*> sliders_;
  ObjectProxyView *tree_view_item_;
};

#endif // MIMAS_SRC_CORE_SIMPLE_OBJECT_PROXY_H_