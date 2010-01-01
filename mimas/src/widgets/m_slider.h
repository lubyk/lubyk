#ifndef MIMAS_SRC_WIDGETS_M_SLIDER_H_
#define MIMAS_SRC_WIDGETS_M_SLIDER_H_
#include "m_observable.h"
#include "m_range_widget.h"

class MimasWindowContent;

class MSlider : public Component, public MRangeWidget {
public:
  MSlider(MimasWindowContent *mimas, const std::string &name)
      : Component(String(name.c_str())),
        mimas_(mimas),
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

  /** Set color hue (must be a value from 0 to 360 degrees).
   */
  void set_hue(float hue) {
    if (hue < 0 || hue >= 360) hue = 0;
    //                     hue           sat  bri  alpha
    border_color_ = Colour(hue / 360.0f, 1.0f, 1.0f, 1.0f);
    fill_color_   = Colour(hue / 360.0f, 0.5f, 0.5f, 1.0f);
  }

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
  MimasWindowContent *mimas_;
  SliderType slider_type_;
  Colour     border_color_;
  Colour     fill_color_;
  ComponentDragger dragger_;

  bool  is_dragged_;
  float slider_pos_;
};

#endif // MIMAS_SRC_WIDGETS_M_SLIDER_H_