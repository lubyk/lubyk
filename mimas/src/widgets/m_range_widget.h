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

#ifndef MIMAS_SRC_WIDGETS_M_RANGE_WIDGET_H_
#define MIMAS_SRC_WIDGETS_M_RANGE_WIDGET_H_
#include "m_observable.h"

#include <iostream>

class MObjectProxy;

class MRangeWidget : public MObservable {
public:
  MRangeWidget();

  virtual ~MRangeWidget() {}

  virtual void set_range(Real min, Real max) {
    min_ = min;
    max_ = max;
  }

  void set_remote_value(Real value) {
    remote_value_ = value;
    redraw();
  }

  void set_value(Real value) {
    value_ = value;
    redraw();
  }

  Real scaled_value(Real range) {
    if (max_ - min_ == 0) return 0;
    return value_ * range / (max_ - min_);
  }

  Real scaled_remote_value(Real range) {
    if (max_ - min_ == 0) return 0;
    return remote_value_ * range / (max_ - min_);
  }

  void set_scaled_value(Real position, Real range);

  void stop_drag();

  void set_proxy(MObjectProxy *proxy);

  int last_drag() {
    return last_drag_;
  }

  /* =======  Callbacks that need to be implemented in sub-classes. ========== */

  /** enable/disable widget.
   */
  virtual void set_enabled(bool enabled) = 0;

  /** Should return true if the component is being dragged.
   */
  virtual bool is_dragged() = 0;

  /** Should provoque a redraw of the widget.
   */
  virtual void redraw() = 0;

protected:

  /** Time when the last dragging operation occured.
   */
  int last_drag_;

  /** Smallest possible value.
   */
  Real min_;

  /** Largest possible value.
   */
  Real max_;

  /** Current value set by user.
   */
  Real value_;

  /** Remote value.
   */
  Real remote_value_;

  /** Object proxy that is connected to this widget.
   */
  MObjectProxy *proxy_;
};

#endif // MIMAS_SRC_WIDGETS_M_RANGE_WIDGET_H_