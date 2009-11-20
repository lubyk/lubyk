#include "oscit/object_proxy.h"
#include "oscit/root.h"
#include "oscit/root_proxy.h"

namespace oscit {

void ObjectProxy::set_root(Root *root) {
  Object::set_root(root);
  root_proxy_ = TYPE_CAST(RootProxy, root);
}

} // oscit