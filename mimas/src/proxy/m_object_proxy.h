#ifndef MIMAS_SRC_PROXY_M_OBJECT_PROXY_H_
#define MIMAS_SRC_PROXY_M_OBJECT_PROXY_H_

#include "oscit/object_proxy.h"
#include "oscit/tobservable_list.h"

#include "object_proxy_tree_item.h"
#include "m_slider.h"


class MObjectProxy : public ObjectProxy, public SliderListener {
public:
  /** Class signature. */
  TYPED("Object.ObjectProxy.MObjectProxy")

  MObjectProxy(const std::string &name, const Value &type) : ObjectProxy(name, type), sliders_(this) {
    set_and_hold(&tree_view_item_, new ObjectProxyTreeItem(this));
  }

  ObjectProxyTreeItem *object_proxy_view() { // FIXME: how to keep constness ok here ?
    return tree_view_item_;
  }

  void observe(ObservableSlider *slider) {
    sliders_.push_back(slider);
    slider->addListener(this);
  }

  virtual void sliderValueChanged (Slider *slider) {
    // user slider being dragged
    set_value(Value(slider->getValue()));
  }

  virtual void 	sliderDragEnded (Slider *slider) {
    slider->setComponentProperty(T("LastDrag"), (int)time_ref_.elapsed());
  }

  virtual void observer_lock() {
    mml_ = new MessageManagerLock();
  }

  virtual void observer_unlock() {
    delete mml_;
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

  /** When we finally know our type, enable the views.
   */
  virtual void type_changed();

  ObservableSlider *build_slider();

private:
  MessageManagerLock *mml_;
  TObservableList<ObservableSlider *> sliders_;
  ObjectProxyTreeItem *tree_view_item_;
};

#endif // MIMAS_SRC_PROXY_M_OBJECT_PROXY_H_