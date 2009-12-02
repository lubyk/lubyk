#include "oscit/object_proxy.h"
#include "oscit/root.h"
#include "oscit/root_proxy.h"

namespace oscit {

void ObjectProxy::set_root(Root *root) {
  Object::set_root(root);
  root_proxy_ = TYPE_CAST(RootProxy, root);
}

void ObjectProxy::set_value(const Value &val) {
  if (can_receive(val)) {
    time_t now = time_ref_.elapsed();
    // only send if value type is correct
    if (!last_delay_ && waiting_for_reply_) {
      // first call: wait
    } if (waiting_for_reply_ && now < waiting_for_reply_) {
      // do not send
      to_send_ = val;
    } else {
      root_proxy_->send_to_remote(url().c_str(), val);
      waiting_for_reply_ = now + last_delay_;
    }
  }
}

void ObjectProxy::handle_value_change(const Value &val) {
  // TODO: root should check type
  value_ = val;
  if (!last_delay_) {
    if (waiting_for_reply_) {
      // we guess we are receiving from our own send...
      // TODO: this can lead to last_delay_ being too short if we receive another
      // notification, but it does not matter
      last_delay_ = time_ref_.elapsed() - waiting_for_reply_;
      waiting_for_reply_ = 0;
    }
  } else {
    if (waiting_for_reply_)
      last_delay_ = last_delay_ + (time_ref_.elapsed() - waiting_for_reply_) / 4;
    if (!to_send_.is_nil()) {
      // send now
      root_proxy_->send_to_remote(url().c_str(), to_send_);
      to_send_ = gNilValue;
      waiting_for_reply_ = time_ref_.elapsed() + last_delay_;
    } else {
      waiting_for_reply_ = 0;
    }
  }
  value_changed();
}
} // oscit