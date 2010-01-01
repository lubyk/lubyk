#include "mimas.h"
#include "m_device_view.h"

#include "m_device_label.h"

MDeviceView::MDeviceView(MimasWindowContent *mimas, const std::string &name)
    : Component(String(name.c_str())),
      mimas_(mimas) {
  label_ = new MDeviceLabel(mimas_, this, T("device name"), String(name.c_str()));

  border_ = new ResizableBorderComponent(this, 0);
  addAndMakeVisible(border_);
  addAndMakeVisible(label_);
  resized();
}

//MDeviceView::~MDeviceView() {
//
//}

void MDeviceView::resized() {
  // FIXME: get label width...
  border_->setBounds(
    (DEVICE_BORDER_WIDTH / 2),
    (DEVICE_BORDER_WIDTH / 2) + label_->min_height() / 2,
    getWidth() - DEVICE_BORDER_WIDTH,
    getHeight() - (label_->min_height() / 2) - DEVICE_BORDER_WIDTH
  );
  //  device_browser_->setBounds (10, 10, getWidth() - 10, getHeight() - 250);
  //  quitButton->setBounds (getWidth() - 176, getHeight() - 60, 120, 32);
  //  workspace_->setBounds (10, getHeight() - 250, getWidth() - 20, 200);
}