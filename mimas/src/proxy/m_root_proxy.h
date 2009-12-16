#ifndef MIMAS_SRC_PROXY_M_ROOT_PROXY_H_
#define MIMAS_SRC_PROXY_M_ROOT_PROXY_H_
#include "oscit/root_proxy.h"
#include "root_proxy_tree_item.h"

class MRootProxy : public RootProxy {
public:
  /** Class signature. */
  TYPED("Object.Root.RootProxy.MRootProxy")

  MRootProxy(const Location &endpoint) : RootProxy(endpoint) {
    set_and_hold(&tree_view_item_, new RootProxyTreeItem(this));
  }

  ~MRootProxy() {
    if (tree_view_item_) {
      MessageManagerLock mml;
      delete tree_view_item_;
    }
  }

  RootProxyTreeItem *tree_view_item() {
    return tree_view_item_;
  }

private:
  RootProxyTreeItem *tree_view_item_;
};

#endif // MIMAS_SRC_PROXY_M_ROOT_PROXY_H_