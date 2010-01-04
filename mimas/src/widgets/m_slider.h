/*
  ==============================================================================

   This file is part of the MIMAS project (http://rubyk.org/mimas)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#ifndef MIMAS_SRC_WIDGETS_M_SLIDER_H_
#define MIMAS_SRC_WIDGETS_M_SLIDER_H_
#include "m_observable.h"
#include "m_component.h"
#include "m_range_widget.h"

class MViewProxy;

class MSlider : public MComponent, public MRangeWidget {
public:
  MSlider(const std::string &part_id, MViewProxy *view_proxy)
      : MComponent(part_id, view_proxy),
        is_dragged_(false) {}

  enum SliderType {
    VerticalSliderType,
    HorizontalSliderType
  };

  void set_slider_type(SliderType type) {
    slider_type_ = type;
  }

  /** =========== MComponent callbacks ========== */

  virtual void update(const Value &def);

  /** Returns true if the slider is connected to a proxy.
   */
  virtual bool is_connected();

  /** =========== MRangeWidget callbacks ========== */

  virtual void set_enabled(bool enabled) {
    setEnabled(enabled);
    set_hue(hue_);
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

  bool  is_dragged_;
  float slider_pos_;
};

#endif // MIMAS_SRC_WIDGETS_M_SLIDER_H_