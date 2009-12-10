#ifndef MIMAS_SRC_CORE_SIMPLE_OBJECT_PROXY_H_
#define MIMAS_SRC_CORE_SIMPLE_OBJECT_PROXY_H_
#include "oscit/object_proxy.h"
#include "object_proxy_view.h"
#include "observable_slider.h"

class SimpleObjectProxy : public ObjectProxy, public SliderListener {
public:
  /** Class signature. */
  TYPED("Object.ObjectProxy.SimpleObjectProxy")

  SimpleObjectProxy(const std::string &name, const Value &type) : ObjectProxy(name, type) {
    set_and_hold(&tree_view_item_, new ObjectProxyView(this));
  }

  ObjectProxyView *object_proxy_view() { // FIXME: how to keep constness ok here ?
    return tree_view_item_;
  }

  void observe(ObservableSlider *slider) {
    append_and_hold(&sliders_, slider);
    if (value_.is_real())
      slider->setComponentProperty(T("RemoteValue"), value_.r);
    slider->addListener(this);
  }

  virtual void sliderValueChanged (Slider *slider) {
    // user slider being dragged
    set_value(Value(slider->getValue()));
  }

  virtual void 	sliderDragEnded (Slider *slider) {
    slider->setComponentProperty(T("LastDrag"), (int)time_ref_.elapsed());
  }

  virtual void value_changed() {
    // update real value slider
    MessageManagerLock mml;
    String remote_value("RemoteValue");
    if (value_.is_real()) {
      std::list<ObservableSlider*>::iterator it, end = sliders_.end();
      for (it = sliders_.begin(); it != end; ++it) {
        (*it)->setComponentProperty(remote_value, value_.r);
        if ((*it)->getThumbBeingDragged() == -1) {
          int last = (*it)->getComponentPropertyDouble(T("LastDrag"), false);
          // no dragging
          if (last < time_ref_.elapsed() - 2 * latency_) {
            (*it)->setValue(value_.r, false);
          } else {
            (*it)->repaint();
          }
        }
      }
    }
  }

  ObservableSlider *build_slider();

private:
  std::list<ObservableSlider*> sliders_;
  ObjectProxyView *tree_view_item_;
};

#endif // MIMAS_SRC_CORE_SIMPLE_OBJECT_PROXY_H_