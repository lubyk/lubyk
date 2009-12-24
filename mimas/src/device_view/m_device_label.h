#ifndef MIMAS_SRC_DEVICE_VIEW_M_DEVICE_LABEL_H_
#define MIMAS_SRC_DEVICE_VIEW_M_DEVICE_LABEL_H_
#include "m_object_proxy.h"

class MDeviceView;

class MDeviceLabel : public Component {
public:
  MDeviceLabel(MDeviceView *device_view, const String &name, const String &device_name);

  virtual void mouseDrag(const MouseEvent &e);

  virtual void mouseDown(const MouseEvent &e);

  int min_width() {
    return min_width_;
  }

  int min_height() {
    return min_height_;
  }
private:
  Label *label_;
  int min_width_;
  int min_height_;
  MDeviceView *device_view_;
  ComponentDragger dragger_;
};

#endif // MIMAS_SRC_DEVICE_VIEW_M_DEVICE_LABEL_H_