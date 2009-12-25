#include "mimas.h"
#include "device_browser.h"

DeviceBrowser::DeviceBrowser(const char *service_type) : ZeroConfBrowser(service_type) {
  std::cout << "new browser with type " << service_type << "\n";
  setRootItem(&device_browser_root_);
  setRootItemVisible(false);

  border_ = new ResizableBorderComponent(this, 0);
  addAndMakeVisible(border_);
}

void DeviceBrowser::added_proxy(RootProxy *proxy) {
  std::cout << "added_proxy " << proxy << " " << proxy->remote_location() << "\n";
  MRootProxy *device = TYPE_CAST(MRootProxy, proxy);
  assert(device);
  MessageManagerLock mml;
  ScopedAutoReleasePool pool;
  device_browser_root_.addSubItem(device->tree_view_item());
}

void DeviceBrowser::removed_proxy(RootProxy *proxy) {
  // find position in tree
  MRootProxy *device = TYPE_CAST(MRootProxy, proxy);
  int index = device_browser_root_.get_item_position(device->tree_view_item());
  MessageManagerLock mml;
  if (index >= 0) device_browser_root_.removeSubItem(index); // do not delete, this will be done by ~MRootProxy
  std::cout << "removed_proxy " << index << " " << proxy << " " << proxy->remote_location() << "\n";
}

void DeviceBrowser::paint(Graphics &g) {
  g.fillAll(Colours::pink);
  g.setColour(MAIN_BORDER_COLOR);
  g.drawLine(
    getWidth() - LAYOUT_BROWSER_BORDER_WIDTH / 2.0f,
    0,
    getWidth() - LAYOUT_BROWSER_BORDER_WIDTH / 2.0f,
    getHeight(),
    LAYOUT_BROWSER_BORDER_WIDTH
  );
  TreeView::paint(g);
}

void DeviceBrowser::resized() {
  //main component ->resized();
  border_->setBounds(0, 0, getWidth(), getHeight());
}
