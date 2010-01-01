#ifndef MIMAS_SRC_DEVICE_BROWSER_DEVICE_BROWSER_H_
#define MIMAS_SRC_DEVICE_BROWSER_DEVICE_BROWSER_H_
#include "oscit/zeroconf_browser.h"
#include "device_browser_root.h"

class MimasWindowContent;
class MDeviceList;
class MBreadcrumbs;

class MBrowser : public Component, public ZeroConfBrowser {
public:
  MBrowser(MimasWindowContent *mimas, const char *service_type);

  virtual void added_proxy(RootProxy *proxy);

  virtual void removed_proxy(RootProxy *proxy);

  virtual void paint(Graphics &g);

  virtual void resized();

private:
  MimasWindowContent *mimas_;

  /** List of found devices.
   */
  MDeviceList *device_list_;

  /** Currently selected device.
   */
  //RootProxy *selected_device_;

  /** Currently selected parent path (this is the
   * parent of the items listed in the children list).
   */
  //MObjectProxy *selected_parent_;

  /** Path hierarchy of the currently selected parent path.
   */
  //MBreadcrumbs *breadcrumbs_;

  /** List of sub-paths in the currently selected path (selected_parent_).
   */
  //MPathList *children_list_;

  /** Right border to resize the width of the device browser.
   */
  ResizableBorderComponent *border_;
};

#endif // MIMAS_SRC_DEVICE_BROWSER_DEVICE_BROWSER_H_