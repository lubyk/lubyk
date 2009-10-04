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
 */
class ObjectProxy : Object {
public:
  ObjectProxy(RootProxy *root_proxy, const char *name, const Value &type) : Object(name, type), root_proxy_(root_proxy), synced_children_(false) {}
  
  /** This method should be called to set a new value for the 'real' remote object. It
   * is usually called by some GUI widget callback when the user changes a control. */
  void set_value(const Value &val) {
    if (can_receive(val)) {
      // only send if value type is correct
      root_proxy_->send_to_remote(url(), value);
    }
  }
  
  /** This method should be implemented in order to give a visual feedback to
   * users that the value has changed (GUI object notification).
   */
  virtual void value_changed(const Value &val) {
    
  }
  
  void handle_list_with_type_reply(const Value &children) {
    if (!children.is_list()) return;
    int max = children.size();
    Object *child;
    Value child_def;
    for(int i = 0; i < max; ++i) {
      child_def = children[i];
      // TODO: make sure child_def is of a proper format !
      child = this->child(child_def[0].str());
      if (!child) {
        adopt(root_proxy_->build_object_proxy(child_def[0].str(), child_def[1]));
      } else {
        // ignore
        // set type ?
      }
    }
  }
  
  void sync(bool forced = false) {
    if (!synced_children_ || forced) {
      root_proxy_->send_to_remote(LIST_PATH));
    }
    synced_children_ = true;
  }
  
private:
  /** Reference to a RootProxy object that links back to the original tree.
  */
  RootProxy *root_proxy_;
  
  bool synced_children_;
};


}

#endif // OSCIT_INCLUDE_OSCIT_OBJECT_PROXY_H_