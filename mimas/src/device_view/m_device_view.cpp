#include "mimas.h"
#include "m_device_view.h"

#include "m_device_label.h"


MDeviceView::MDeviceView(const std::string &name)
    : Component(String(name.c_str())) {
  label_ = new MDeviceLabel(this, T("device name"), String(name.c_str()));
  addAndMakeVisible(label_);
  resized();
}

//MDeviceView::~MDeviceView() {
//
//}

void MDeviceView::paint(Graphics &g) {
  if (hover_) {
    g.fillAll(Colour(0xffeeeeee));
  }
  g.setColour(Colours::grey);
  g.drawRect(0, label_->min_height() / 2, getWidth(), getHeight() - (label_->min_height() / 2));
}

void MDeviceView::resized() {
  // FIXME: get label width...
  //  device_browser_->setBounds (10, 10, getWidth() - 10, getHeight() - 250);
  //  quitButton->setBounds (getWidth() - 176, getHeight() - 60, 120, 32);
  //  main_view_->setBounds (10, getHeight() - 250, getWidth() - 20, 200);
}