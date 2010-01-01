#include "mimas.h"
#include "m_device_list.h"
#include "mimas_window_content.h"

MDeviceList::MDeviceList(MimasWindowContent *mimas)
  : ListBox(T("device list"), NULL),
    ListBoxModel(),
    mimas_(mimas) {

  setModel(this);
  setMultipleSelectionEnabled(false);

  // TODO: should replace this with a callback on theme change...
  setColour(ListBox::backgroundColourId, mimas_->bg_color());
  setColour(ListBox::outlineColourId, mimas_->color(MTheme::BrowserSelectedItem));
  setColour(ListBox::textColourId, mimas_->color(MTheme::DeviceLabel));
}

// ======== data management        =========== //
void MDeviceList::add_device(RootProxy *device) {
  devices_.push_back(device);
  updateContent();
}

void MDeviceList::remove_device(RootProxy *device) {
  std::vector<RootProxy*>::iterator it, end = devices_.end();
  for(it = devices_.begin(); it != end; ++it) {
    if (*it == device) {
      it = devices_.erase(it);
    }
  }
  updateContent();
}
// ======== ListBoxModel callbacks =========== //

int MDeviceList::getNumRows() {
  std::cout << "getNumRows = " << devices_.size() << "\n";
  return devices_.size();
}
