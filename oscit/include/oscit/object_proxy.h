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

/** Copyright 2009 Gaspard Bucher
 *
 */

#ifndef OSCIT_INCLUDE_OSCIT_OBJECT_PROXY_H_
#define OSCIT_INCLUDE_OSCIT_OBJECT_PROXY_H_

#include "oscit/object.h"
#include "oscit/root_proxy.h"
#include "oscit/time_ref.h"

namespace oscit {

/** This class helps maintain a 'ghost' object that mirrors a remote 'real' object. It is usually used
 * as an element from an interface to interact with the remote object.
 *
 * This object is not fully functional before the root_proxy has been set (when adopted by a RootProxy).
 */
class ObjectProxy : public Object {
public:
  /** Class signature. */
  TYPED("Object.ObjectProxy")

  ObjectProxy(const char *name, const Value &type) :
              Object(name, type), need_sync_(true), root_proxy_(NULL), wait_until_(-1), latency_(-1) {
    if (type.is_list()) {
      value_ = type[0];
    }
  }

  ObjectProxy(const std::string &name, const Value &type) :
              Object(name, type), need_sync_(true), root_proxy_(NULL), wait_until_(-1), latency_(-1) {
    if (type.is_list()) {
      value_ = type[0];
    }
  }

  virtual ~ObjectProxy() {}

  /** This method should be called to set a new value for the 'real' remote object. It
   * is usually called by some GUI widget callback when the user changes a control. */
  void set_value(const Value &val);


  virtual const Value trigger(const Value &val) {
    if (!val.is_nil()) {
      set_value(val);
    }
    return value_;
  }

  /** This method should be implemented in order to give a visual feedback to
   * users that the value has changed (GUI object notification).
   */
  virtual void value_changed() {
  }

  /** This method is called when the object proxy receives it's type information or the type
   * of the remote changed.
   */
  virtual void type_changed() {

  }

  /** Find the list of children by querying the remote.
   */
  void sync_children(bool forced = false) {
    if (need_sync_ || forced) {
      root_proxy_->send_to_remote(LIST_WITH_TYPE_PATH, Value(url()));
    }
    need_sync_ = false;
  }

  /** Set root and if the root is a RootProxy, set root_proxy_ as well. You can
   * overwrite this (but make sure to call ObjectProxy::set_root(root) in your
   * method) but you should not call it directly. It is called when the object
   * is adopted.
   */
  virtual void set_root(Root *root);

  /** Returns true if the object has been properly initialized (type and value
   * have been synced with remote).
   */
  bool is_connected() const {
    return !type_.is_nil();
  }

  /** @internal.
   * Used by root_proxy_ when initializing the object.
   */
  void set_need_sync(bool need_sync) {
    need_sync_ = need_sync;
  }

  /** @internal.
   * Called when the object proxy finally receives type information.
   */
  void set_type(const Value &type);

  /** @internal.
   * Method triggered on a value change notification. This method calls
   * sets the new value in cache and calls "value_changed".
   */
  void handle_value_change(const Value &val);

  /** @internal.
   * Dynamically build a child from the given name. If type is empty, we build dummy
   * object proxies that will try to get a "type" from the remote end.
   */
  virtual Object *build_child(const std::string &name, const Value &type, Value *error);

  TimeRef &time_ref() {
    return time_ref_;
  }
protected:


  bool need_sync_;

  /** Reference to a RootProxy object that links back to the original tree.
  */
  RootProxy *root_proxy_;

  /** Cached information on the remote value.
   */
  Value value_;

  /** We have sent a notification out and the reply has not come back yet, we need to slow
   * down in order to avoid flooding.
   */
  time_t wait_until_;

  /** We have sent a notification out and the reply has not come back yet, we need to slow
   * down in order to avoid flooding.
   */
  time_t latency_;

  Value to_send_;

  TimeRef time_ref_;
};


}

#endif // OSCIT_INCLUDE_OSCIT_OBJECT_PROXY_H_