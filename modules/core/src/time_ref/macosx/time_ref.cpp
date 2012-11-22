/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
   Copyright (c) 2007-2011 by Gaspard Bucher (http://teti.ch).

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

#include <assert.h>
//#include <CoreServices/CoreServices.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>
#include <stdio.h>

#include "lk/TimeRef.h"

namespace lk {
struct TimeRef::TimeRefData {
  uint64_t mach_ref_;
  double   mach_convert_;
};


TimeRef::TimeRef() {
  mach_timebase_info_data_t time_base_info;
  reference_ = new TimeRefData;
  mach_timebase_info(&time_base_info);
  // numer/denom converts to nanoseconds. We multiply by 10^6 to have milliseconds
  reference_->mach_convert_ = (double)time_base_info.numer / (time_base_info.denom * 1000000);
  reference_->mach_ref_ = mach_absolute_time();
}

TimeRef::~TimeRef() {
  delete reference_;
}

/** Get current real time in [ms] since the time ref object was created.
 */
double TimeRef::elapsed() {
  return reference_->mach_convert_ * (mach_absolute_time() - reference_->mach_ref_);
}
} // lk
