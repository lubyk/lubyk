#include "mimas.h"
#include "m_browser.h"
#include "m_device_list.h"
#include "mimas_window_content.h"

MBrowser::MBrowser(MimasWindowContent *mimas, const char *service_type)
    : ZeroConfBrowser(service_type),
      mimas_(mimas) {

  device_list_ = new MDeviceList(mimas_);
  addAndMakeVisible(device_list_);

  border_ = new ResizableBorderComponent(this, 0);
  addAndMakeVisible(border_);
}

void MBrowser::added_proxy(RootProxy *proxy) {
  MessageManagerLock mml;
  ScopedAutoReleasePool pool;
  device_list_->add_device(proxy);
}

void MBrowser::removed_proxy(RootProxy *proxy) {
  MessageManagerLock mml;
  ScopedAutoReleasePool pool;
  device_list_->remove_device(proxy);
}

void MBrowser::resized() {
  device_list_->setBounds(0, 0, getWidth() - LAYOUT_BROWSER_BORDER_WIDTH, getHeight() / 3.0f);
  border_->setBounds(0, 0, getWidth(), getHeight());
  mimas_->resize_except(this);
}
