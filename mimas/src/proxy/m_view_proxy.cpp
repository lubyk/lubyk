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
#include "m_view_proxy.h"

#include <string>
#include <iostream>

#include "mimas_window_content.h"
#include "m_device_view.h"

/** This class responds to the "update" method in a view and is
 * responsible for building/updating the view accordingly.
 */
class MUpdateViewProxy : public MObjectProxy {
public:
  MUpdateViewProxy(MViewProxy *view_proxy, const Value &type)
      : MObjectProxy(view_proxy->mimas(), std::string("update"), type),
        view_proxy_(view_proxy) {}

  /** "update" method inside the view.
   */
  virtual void value_changed() {
    MessageManagerLock mml;
    view_proxy_->update_view(value_);
  }

private:
  MViewProxy *view_proxy_;
};


// ==================================== MViewProxy =============

MViewProxy::MViewProxy(MimasWindowContent *mimas, const std::string &name, const Value &type)
    : MObjectProxy(mimas, name, type),
      view_(NULL),
      update_view_proxy_(NULL) {}


MViewProxy::~MViewProxy() {
  MessageManagerLock mml;
  if (view_) delete view_;
}

void MViewProxy::adopted() {
  ObjectProxy::adopted();
  if (root_proxy_) {
    sync_children();

    MessageManagerLock mml;
    mimas_->workspace()->addAndMakeVisible(view());
  }
}

Object *MViewProxy::build_child(const std::string &name, const Value &type, Value *error) {
  if (name == "update") {
    std::cout << "build 'update' with type: " << type << "\n";
    update_view_proxy_ = adopt(new MUpdateViewProxy(this, type));
    return update_view_proxy_;
  } else {
    return NULL;
  }
}

// widget callback
void MViewProxy::update_remote(const Value& hash) {
  if (update_view_proxy_) {
    std::cout << "update_view_proxy_! " << hash << "\n";
    update_view_proxy_->set_value(hash);
  } else {
    Value def(value_.to_json()); // copy
    def.deep_merge(hash);
    set_value(def);
  }
}

/** Build or update the widgets on a view from the view definitions.
 */
void MViewProxy::update_view(const Value &def) {
  if (!view_) {
    view_ = new MDeviceView(std::string(""), this, root_proxy_->remote_location().name());
  }

  if (!def.is_hash()) {
    std::cerr << "'" << url() << "' cannot update view: parameter is not a Hash: " << def << "\n";
    assert(false);
    return;
  }

  view_->update(def);
}


