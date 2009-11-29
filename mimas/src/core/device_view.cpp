#include "mimas.h"
#include "device_view.h"
#include "device_proxy.h"

DeviceView::DeviceView(DeviceProxy *proxy) {
  set_and_hold(&device_proxy_, proxy);
}

void DeviceView::paintItem(Graphics& g, int width, int height) {
    // if this item is selected, fill it with a background colour..
  if (isSelected()) g.fillAll(Colours::blue.withAlpha (0.3f));
  // use a "colour" attribute in the xml tag for this node to set the text colour..
  g.setColour (Colours::black);

  g.setFont (height * 0.7f);

  // draw the xml element's tag name..
  g.drawText(String(device_proxy_->remote_location().name().c_str()),
    4, 0, width - 4, height,
    Justification::centredLeft, true);
}
