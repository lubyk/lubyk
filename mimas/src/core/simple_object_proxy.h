#ifndef MIMAS_SRC_CORE_SIMPLE_OBJECT_PROXY_H_
#define MIMAS_SRC_CORE_SIMPLE_OBJECT_PROXY_H_
#include "oscit/object_proxy.h"
#include "object_proxy_view.h"
#include "observable_slider.h"

class SimpleObjectProxy : public ObjectProxy, public SliderListener {
public:
  SimpleObjectProxy(const std::string &name, const Value &type) : ObjectProxy(name, type), slider_(NULL) {
    set_and_hold(&tree_view_item_, new ObjectProxyView(this));
  }

  ObjectProxyView *object_proxy_view() const { // FIXME: how to keep constness ok here ?
    return tree_view_item_;
  }

  void set_slider(ObservableSlider *slider) {
    set_and_hold(&slider_, slider);
  }

  virtual void sliderValueChanged (Slider *slider) {
    // user slider being dragged
    set_value(Value(slider->getValue()));
  }

  virtual void 	sliderDragEnded (Slider *slider) {
    // sync user slider with real value
    if (value_.is_real()) {
      slider->setValue(value_.r, false); // do not notify
    }
  }

  virtual void value_changed() {
    // update real value slider
    MessageManagerLock mml;
    if (slider_ && value_.is_real()) {
      // request a redraw... ?
      slider_->setValue(value_.r, false);
    }
  }

private:
  ObservableSlider *slider_;
  ObjectProxyView *tree_view_item_;
};

#endif // MIMAS_SRC_CORE_SIMPLE_OBJECT_PROXY_H_