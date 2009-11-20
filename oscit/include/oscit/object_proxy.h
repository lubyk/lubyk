/** Copyright 2009 Gaspard Bucher
 *
 */

#ifndef OSCIT_INCLUDE_OSCIT_OBJECT_PROXY_H_
#define OSCIT_INCLUDE_OSCIT_OBJECT_PROXY_H_

#include "oscit/object.h"
#include "oscit/root_proxy.h"

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
              Object(name, type), synced_children_(false), root_proxy_(NULL) {}

  ObjectProxy(const std::string &name, const Value &type) :
              Object(name, type), synced_children_(false), root_proxy_(NULL) {}

  /** This method should be called to set a new value for the 'real' remote object. It
   * is usually called by some GUI widget callback when the user changes a control. */
  void set_value(const Value &val) {
    if (can_receive(val)) {
      // only send if value type is correct
      root_proxy_->send_to_remote(url().c_str(), val);
    }
  }

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

  void handle_value_change(const Value &val) {
    // TODO: root should check type
    value_ = val;
    value_changed();
  }

  void sync(bool forced = false) {
    if (!synced_children_ || forced) {
      root_proxy_->send_to_remote(LIST_WITH_TYPE_PATH, Value(url()));
    }
    synced_children_ = true;
  }

  /** Set root and if the root is a RootProxy, set root_proxy_ as well.
   */
  virtual void set_root(Root *root);

protected:

  bool synced_children_;

  /** Reference to a RootProxy object that links back to the original tree.
  */
  RootProxy *root_proxy_;

  /** Cached information on the remote value.
   */
  Value value_;
};


}

#endif // OSCIT_INCLUDE_OSCIT_OBJECT_PROXY_H_