#ifndef MIMAS_SRC_CORE_DEVICE_PROXY_H_
#define MIMAS_SRC_CORE_DEVICE_PROXY_H_
#include "oscit/root_proxy.h"
#include "device_view.h"

class DeviceProxy : public RootProxy {
public:
  DeviceProxy(const Location &endpoint) : RootProxy(endpoint) {
    set_and_hold(&tree_view_item_, new DeviceView(this));
  }

  ~DeviceProxy() {
    if (tree_view_item_) {
      MessageManagerLock mml;
      delete tree_view_item_;
    }
  }

  DeviceView *tree_view_item() {
    return tree_view_item_;
  }

private:
  DeviceView *tree_view_item_;
};

#endif // MIMAS_SRC_CORE_DEVICE_PROXY_H_