#include "mimas.h"
#include "device_view.h"
#include "device_proxy.h"
#include "simple_object_proxy.h"

DeviceView::DeviceView(DeviceProxy *proxy) : subnodes_added_(false) {
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

void DeviceView::itemOpennessChanged(bool isNowOpen) {
  if (isNowOpen && !subnodes_added_) {
    // FIXME: make sure we do not accept changes to this tree now !
    const THash<std::string, Object *> children = device_proxy_->children();
    std::list<std::string>::const_iterator it = children.begin();
    std::list<std::string>::const_iterator end = children.end();
    for (; it != end; ++it) {
      std::cout << *it << " <<< added to device\n";
      Object *object;
      assert(children.get(*it, &object));
      SimpleObjectProxy *object_proxy = TYPE_CAST(SimpleObjectProxy, object);
      assert(object_proxy);
      object_proxy->sync();
      addSubItem(object_proxy->object_proxy_view());
    }
    subnodes_added_ = true;
  }
}