#ifndef MIMAS_SRC_WIDGETS_M_SLIDER_H_
#define MIMAS_SRC_WIDGETS_M_SLIDER_H_
#include "m_observable.h"
#include "m_component.h"
#include "m_range_widget.h"

class MViewProxy;

class MSlider : public MComponent, public MRangeWidget {
public:
  MSlider(MViewProxy *view_proxy, const Value &def)
      : MComponent(view_proxy, def),
        is_dragged_(false) {
    set_hue(182);
  }

  enum SliderType {
    VerticalSliderType,
    HorizontalSliderType
  };

  void set_slider_type(SliderType type) {
    slider_type_ = type;
  }

  /** =========== MComponent callbacks ========== */

  virtual void update(const Value &def);

  /** =========== MRangeWidget callbacks ========== */

  virtual void set_enabled(bool enabled) {
    setEnabled(enabled);
  }

  virtual void redraw() {
    repaint();
  }

  virtual bool is_dragged() {
    return is_dragged_;
  }

  /** =========== Component callbacks  ========== */

  virtual void mouseDown(const MouseEvent &e);

  virtual void mouseDrag(const MouseEvent &e);

  virtual void mouseUp(const MouseEvent &e);

  virtual void paint(Graphics &g);

  // virtual void valueChanged() {
  //   // user slider being dragged
  //   proxy_->set_value(Value(getValue()));
  // }
  //
  // virtual void stoppedDragging() {
  //   last_drag_ = proxy_->time_ref().elapsed();
  // }
private:
  SliderType slider_type_;
  ComponentDragger dragger_;

  bool  is_dragged_;
  float slider_pos_;
};

#endif // MIMAS_SRC_WIDGETS_M_SLIDER_H_