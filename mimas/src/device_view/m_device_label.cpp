#include "mimas.h"
#include "m_device_label.h"

#include "m_device_view.h"
#include "mimas_window_content.h"

#define LABEL_MARGIN_T 0

MDeviceLabel::MDeviceLabel(MimasWindowContent *mimas, MDeviceView *device_view, const String &name, const String &device_name)
    : mimas_(mimas),
      device_view_(device_view) {
  label_ = new Label(name, device_name);
  label_->setFont(Font(15.0f, Font::bold));
  label_->setInterceptsMouseClicks(false, false);
  min_width_  = label_->getFont().getStringWidth(label_->getText());
  min_height_ = label_->getFont().getHeight();

  label_->setBounds(DEVICE_ROUNDED_RADIUS * 2.0, LABEL_MARGIN_T, min_width_, min_height_);
  setBounds(0, 0, min_width_ + 2 * DEVICE_ROUNDED_RADIUS * 2.0, min_height_ + 2 * LABEL_MARGIN_T);

  addAndMakeVisible(label_);

  setInterceptsMouseClicks(true, true);
}

void MDeviceLabel::mouseDown(const MouseEvent &e) {
  dragger_.startDraggingComponent(device_view_, 0);
}

void MDeviceLabel::mouseDrag(const MouseEvent &e) {
  dragger_.dragComponent(device_view_, e);
}


