#ifndef MIMAS_SRC_PROXY_M_VIEWS_BUILDER_H_
#define MIMAS_SRC_PROXY_M_VIEWS_BUILDER_H_
#include "m_object_proxy.h"

class MViewsBuilder : public MObjectProxy {
public:
  MViewsBuilder(const std::string &name, const Value &type) : MObjectProxy(name, type) {}

  /** Set root and sync (do not wait for the browser to be opened).
   */
  virtual void set_root(Root *root) {
    ObjectProxy::set_root(root);
    sync_children();
  }
};

#endif // MIMAS_SRC_PROXY_M_VIEWS_BUILDER_H_