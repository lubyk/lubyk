#ifndef MIMAS_SRC_CORE_SIMPLE_OBJECT_PROXY_H_
#define MIMAS_SRC_CORE_SIMPLE_OBJECT_PROXY_H_
#include "oscit/object_proxy.h"
#include "object_proxy_view.h"

class SimpleObjectProxy : public ObjectProxy {
public:
  SimpleObjectProxy(const std::string &name, const Value &type) : ObjectProxy(name, type) {
    set_and_hold(&tree_view_item_, new ObjectProxyView(this));
  }

  ObjectProxyView *object_proxy_view() const { // FIXME: how to keep constness ok here ?
    return tree_view_item_;
  }

private:
  ObjectProxyView *tree_view_item_;
};

#endif // MIMAS_SRC_CORE_SIMPLE_OBJECT_PROXY_H_