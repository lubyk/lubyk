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

#include "oscit/object_proxy.h"

#include "oscit/root.h"
#include "oscit/root_proxy.h"
#include "oscit/proxy_factory.h"

namespace oscit {

void ObjectProxy::set_root(Root *root) {
  Object::set_root(root);
  root_proxy_ = TYPE_CAST(RootProxy, root);
  if (root_proxy_ && type().is_nil()) {
    // try to find type
    root_proxy_->send_to_remote(TYPE_PATH, Value(url()));
  }
}

void ObjectProxy::set_value(const Value &val) {
  if (can_receive(val)) {
    time_t now = time_ref_.elapsed();
    // only send if value type is correct
    if (latency_ < 0) {
      if (wait_until_ >= 0) {
        // waiting for first call reply: wait
      } else {
        // first call
        root_proxy_->send_to_remote(url().c_str(), val);
        wait_until_ = now;
      }
    } else if (now < wait_until_) {
      // do not send
      to_send_ = val;
    } else {
      root_proxy_->send_to_remote(url().c_str(), val);
      wait_until_ = now + (latency_ * 1.5);
    }
  }
}

void ObjectProxy::handle_value_change(const Value &val) {
  // TODO: root should check type
  value_ = val;
  if (latency_ < 0 && wait_until_ >= 0) {
    // latency not set but we have already sent a value out

    // we guess we are receiving from our own send...
    // TODO: this can lead to latency_ being too short if we receive another
    // notification, but it does not matter
    latency_ = time_ref_.elapsed() - wait_until_;
    wait_until_ = 0;
  } else {
    if (wait_until_) {
      // (gently) update latency
      latency_ = latency_ + (time_ref_.elapsed() - wait_until_) / 4;
    }

    if (!to_send_.is_empty()) {
      // send now
      root_proxy_->send_to_remote(url().c_str(), to_send_);
      to_send_.set_empty();
      wait_until_ = time_ref_.elapsed() + (latency_ * 1.5);
    } else {
      wait_until_ = 0;
    }
  }
  value_changed();
}

void ObjectProxy::set_type(const Value &type) {
  if (type != type_) {
    type_ = type;

    if (type_.is_list()) {
      value_ = type[0];
    }

    sync_type_id();
    type_changed();
    value_changed();
  }
}

Object *ObjectProxy::build_child(const std::string &name, const Value &type, Value *error) {
  if (!root_proxy_ || !root_proxy_->proxy_factory()) {
    std::cerr << "Cannot build child /" << name << " : no RootProxy or no ProxyFactory !\n";
    return NULL;
  }
  return adopt(root_proxy_->proxy_factory()->build_object_proxy(this, name, type));
}

} // oscit