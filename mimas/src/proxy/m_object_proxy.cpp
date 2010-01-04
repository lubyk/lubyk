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
#include "m_object_proxy.h"

#include <list>

void MObjectProxy::connect(MRangeWidget *widget) {
  if (is_connected()) {
    std::cout << url() << " connect. Type is: " << type() << "\n\n";
    widget->set_range(type()[1].r, type()[2].r);
    if (value_.is_real()) widget->set_remote_value(value_.r);
  } else {
    widget->set_enabled(false);
  }
  range_widgets_.push_back(widget);
  widget->set_proxy(this);
}

void MObjectProxy::type_changed() {
  if (type().type_id() == H("fffss")) {
    MessageManagerLock mml;
    // RangeIO
    std::list<MRangeWidget*>::iterator it, end = range_widgets_.end();
    for (it = range_widgets_.begin(); it != end; ++it) {
      (*it)->set_enabled(true);
      (*it)->set_range(type()[1].r, type()[2].r);
    }
  }
}