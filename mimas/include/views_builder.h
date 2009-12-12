#ifndef MIMAS_SRC_CORE_VIEW_BUILDER_H_
#define MIMAS_SRC_CORE_VIEW_BUILDER_H_
#include "simple_object_proxy.h"

class ViewsBuilder : public SimpleObjectProxy {
public:
  ViewsBuilder(const std::string &name, const Value &type) : SimpleObjectProxy(name, type) {}

  // /** Set root and sync (do not wait for the browser to be opened).
  //  */
  // virtual void set_root(Root *root) {
  //   ObjectProxy::set_root(root);
  //   sync();
  // }
};

#endif // MIMAS_SRC_CORE_VIEW_BUILDER_H_