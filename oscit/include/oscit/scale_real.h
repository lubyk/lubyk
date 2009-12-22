/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
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

#ifndef OSCIT_INCLUDE_OSCIT_SCALE_REAL_H_
#define OSCIT_INCLUDE_OSCIT_SCALE_REAL_H_
#include <assert.h>
#include <string>

#include "oscit/conf.h"

namespace oscit {

class ScaleReal {
 public:
  ScaleReal(Real source_min, Real source_max, Real target_min, Real target_max) :
                       source_min_(source_min), target_min_(target_min),
                       target_max_(target_max) {
    assert(source_min != source_max);
    scale_ = (target_max - target_min) / (source_max - source_min);
  }
  
  Real scale(Real source_value) const {
    Real res = target_min_ + ( (source_value - source_min_) * scale_);
    return res < target_min_ ? target_min_ : (res > target_max_ ? target_max_ : res);
  }
 
 private:
  Real source_min_;
  Real target_min_;
  Real target_max_;
  Real scale_;
};

}  // oscit

#endif // OSCIT_INCLUDE_OSCIT_SCALE_REAL_H_