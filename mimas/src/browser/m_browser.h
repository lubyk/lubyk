#ifndef MIMAS_SRC_DEVICE_BROWSER_DEVICE_BROWSER_H_
#define MIMAS_SRC_DEVICE_BROWSER_DEVICE_BROWSER_H_
#include "oscit/zeroconf_browser.h"
#include "m_root_proxy.h"
#include "device_browser_root.h"

class DeviceBrowser : public TreeView, public ZeroConfBrowser {
public:
  DeviceBrowser(const char *service_type);

  virtual void added_proxy(RootProxy *proxy);

  virtual void removed_proxy(RootProxy *proxy);

  virtual void paint(Graphics &g);

  virtual void resized();
private:
  DeviceBrowserRoot device_browser_root_;
  ResizableBorderComponent *border_;
};

#endif // MIMAS_SRC_DEVICE_BROWSER_DEVICE_BROWSER_H_