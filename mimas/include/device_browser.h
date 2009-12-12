#ifndef MIMAS_SRC_CORE_DEVICE_BROWSER_H_
#define MIMAS_SRC_CORE_DEVICE_BROWSER_H_
#include "oscit/zeroconf_browser.h"
#include "device_proxy.h"
#include "device_browser_root.h"

class DeviceBrowser : public TreeView, public ZeroConfBrowser {
public:
  DeviceBrowser(const char *service_type) : ZeroConfBrowser(service_type) {
    std::cout << "new browser with type " << service_type << "\n";
    setRootItem(&device_browser_root_);
    setRootItemVisible(false);
  }

  virtual void added_proxy(RootProxy *proxy) {
    std::cout << "added_proxy " << proxy << " " << proxy->remote_location() << "\n";
    DeviceProxy *device = TYPE_CAST(DeviceProxy, proxy);
    assert(device);
    MessageManagerLock mml;
    ScopedAutoReleasePool pool;
    device_browser_root_.addSubItem(device->tree_view_item());
  }

  virtual void removing_proxy(RootProxy *proxy) {
    // find position in tree
    DeviceProxy *device = TYPE_CAST(DeviceProxy, proxy);
    int index = device_browser_root_.get_item_position(device->tree_view_item());
    MessageManagerLock mml;
    if (index >= 0) device_browser_root_.removeSubItem(index); // do not delete, this will be done by ~DeviceProxy
    std::cout << "removing_proxy " << index << " " << proxy << " " << proxy->remote_location() << "\n";
  }

private:
  DeviceBrowserRoot device_browser_root_;

};

#endif // MIMAS_SRC_CORE_OSC_BROWSER_H_