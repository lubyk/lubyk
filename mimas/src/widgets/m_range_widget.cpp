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

#include "mimas.h"
#include "m_range_widget.h"

#include "m_object_proxy.h"


MRangeWidget::MRangeWidget()
    : last_drag_(0),
      min_(0.0),
      max_(1.0),
      value_(0.0),
      remote_value_(0.0),
      proxy_(NULL) {

}

void MRangeWidget::set_proxy(MObjectProxy *proxy) {
  set_and_hold(&proxy_, proxy);
}

void MRangeWidget::set_scaled_value(Real position, Real range) {
  if (!range) return;
  Real r = position * (max_ - min_) / range;
  if (r < min_) r = min_;
  if (r > max_) r = max_;
  value_ = r;
  proxy_->set_value(Value(value_));
}

void MRangeWidget::stop_drag() {
  last_drag_ = proxy_->time_ref().elapsed();
}
