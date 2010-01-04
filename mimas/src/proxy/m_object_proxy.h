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

#ifndef MIMAS_SRC_PROXY_M_OBJECT_PROXY_H_
#define MIMAS_SRC_PROXY_M_OBJECT_PROXY_H_

#include "oscit/object_proxy.h"
#include "oscit/tobservable_list.h"

#include "m_range_widget.h"

class MimasWindowContent;

class MObjectProxy : public ObjectProxy {
public:
  /** Class signature. */
  TYPED("Object.ObjectProxy.MObjectProxy")

  MObjectProxy(MimasWindowContent *mimas, const std::string &name, const Value &type)
      : ObjectProxy(name, type),
        mimas_(mimas),
        range_widgets_(this) {}

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
      std::list<MRangeWidget*>::iterator it, end = range_widgets_.end();
      for (it = range_widgets_.begin(); it != end; ++it) {
        (*it)->set_remote_value(value_.r);
        // FIXME: shouldn't this logic live in the range widget thing ?
        if (!(*it)->is_dragged()) {
          // no dragging
          if ((*it)->last_drag() + (3 * latency_) < time_ref_.elapsed()) {
            // we stopped dragging long ago, move real.
            (*it)->set_value(value_.r);
          }
        }
      }
    }
  }

  /** When we finally know our type, enable the views.
   */
  virtual void type_changed();

  void connect(MRangeWidget *widget);

protected:
  MimasWindowContent *mimas_;
private:
  MessageManagerLock *mml_;
  TObservableList<MRangeWidget *> range_widgets_;
};

#endif // MIMAS_SRC_PROXY_M_OBJECT_PROXY_H_