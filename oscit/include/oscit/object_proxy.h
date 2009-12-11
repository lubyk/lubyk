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

  virtual const Value trigger(const Value &val, const Location *origin) {
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

  void handle_value_change(const Value &val);

  void sync(bool forced = false) {
    if (need_sync_ || forced) {
      root_proxy_->send_to_remote(LIST_WITH_TYPE_PATH, Value(url()));
    }
    need_sync_ = false;
  }

  void set_need_sync(bool need_sync) {
    need_sync_ = need_sync;
  }

  /** Set root and if the root is a RootProxy, set root_proxy_ as well.
   */
  virtual void set_root(Root *root);

  /** @internal. */
  void set_type(const Value &type);

  /** Dynamically build a child from the given name. We build dummy object proxies
   * that will try to get a "type" from the remote end.
   */
  virtual Object *build_child(const std::string &name, Value *error);

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