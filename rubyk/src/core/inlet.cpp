#include "inlet.h"
#include "outlet.h"
#include "node.h"

void Inlet::register_in_node() {
  node_->register_inlet(this);
}

void Inlet::unregister_in_node() {
  node_->unregister_inlet(this);
}

const Value Inlet::trigger(const Value &val) {
  receive(val);
  return node_->do_inspect();
}
