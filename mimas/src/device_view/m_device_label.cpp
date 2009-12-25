#include "mimas.h"
#include "m_device_label.h"

#include "m_device_view.h"

#define LABEL_MARGIN_T 0

MDeviceLabel::MDeviceLabel(MDeviceView *device_view, const String &name, const String &device_name)
    : device_view_(device_view) {
  label_ = new Label(name, device_name);
  label_->setFont(Font(15.0f, Font::bold));
  label_->setColour(Label::textColourId, MAIN_LABEL_COLOR);
  label_->setColour(Label::backgroundColourId, MAIN_BG_COLOR);
  label_->setInterceptsMouseClicks(false, false);
  min_width_  = label_->getFont().getStringWidth(label_->getText());
  min_height_ = label_->getFont().getHeight();

  label_->setBounds(DEVICE_ROUNDED_RADIUS * 2.0, LABEL_MARGIN_T, min_width_, min_height_);
  setBounds(0, 0, min_width_ + 2 * DEVICE_ROUNDED_RADIUS * 2.0, min_height_ + 2 * LABEL_MARGIN_T);

  addAndMakeVisible(label_);

  setInterceptsMouseClicks(true, true);
}

void MDeviceLabel::mouseDrag(const MouseEvent &e) {
  dragger_.dragComponent(device_view_, e);
}


void MDeviceLabel::mouseDown(const MouseEvent &e) {
  std::cout << "Drag\n";
  dragger_.startDraggingComponent(device_view_, 0);
}
