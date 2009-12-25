#ifndef MIMAS_SRC_DEVICE_VIEW_M_DEVICE_VIEW_H_
#define MIMAS_SRC_DEVICE_VIEW_M_DEVICE_VIEW_H_
#include "m_object_proxy.h"

class MDeviceLabel;

class MDeviceView : public Component {
public:
  MDeviceView(const std::string &name);

  //virtual ~MDeviceView();
  virtual void mouseEnter(const MouseEvent &e) {
    hover_ = true;
    repaint();
  }

  virtual void mouseExit(const MouseEvent &e) {
    std::cout << e.x << "x" << e.y << " out\n";
    if (e.x <= 0 || e.x >= getWidth() || e.y <= 0 || e.y >= getHeight()) hover_ = false;
    repaint();
  }

  virtual void 	paint (Graphics &g);

  void resized();

private:
  /** Display the name of the device.
   */
  MDeviceLabel *label_;
  ResizableBorderComponent *border_;
  bool hover_;
};

#endif // MIMAS_SRC_DEVICE_VIEW_M_DEVICE_VIEW_H_