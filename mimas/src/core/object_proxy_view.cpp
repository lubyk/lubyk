#include "mimas.h"
#include "object_proxy_view.h"
#include "simple_object_proxy.h"

ObjectProxyView::ObjectProxyView(SimpleObjectProxy *object) : subnodes_added_(false) {
  set_and_hold(&object_proxy_, object);
}

void ObjectProxyView::paintItem(Graphics& g, int width, int height) {
    // if this item is selected, fill it with a background colour..
  if (isSelected()) g.fillAll(Colours::blue.withAlpha (0.3f));
  // use a "colour" attribute in the xml tag for this node to set the text colour..
  g.setColour (Colours::black);

  g.setFont (height * 0.7f);

  // draw the xml element's tag name..
  g.drawText(String(object_proxy_->name().c_str()),
    4, 0, width - 4, height,
    Justification::centredLeft, true);
}

bool ObjectProxyView::mightContainSubItems() {
  return object_proxy_->children().size() > 0;
}

Component *ObjectProxyView::createItemComponent() {
  Value type = object_proxy_->type();
  std::cout << "type: " << type.type_tag() << "\n";
  if (type.type_id() == H("fffss")) {
    Value current = object_proxy_->trigger(gNilValue, NULL);
    std::cout << "Building slider!\n";
    ObservableSlider *slider;
    Component *component = new Component;
    for(int i = 0; i < 2; ++i) {
      slider = new ObservableSlider(String(object_proxy_->name().c_str()));
      slider->setRange(type[1].r, type[2].r);
      slider->setValue(current.r);
      slider->setSliderStyle(i == 0 ? Slider::LinearHorizontal : Slider::Rotary);
      slider->setTextBoxStyle(Slider::TextBoxLeft, false, 80, 20);
      component->addAndMakeVisible(slider);
      slider->setBounds (100 + i * 150, 2, 150, 20);
      object_proxy_->observe(slider);
    }

    return component;
  } else {
    return 0; // use paintItem instead
  }
}

void ObjectProxyView::itemOpennessChanged(bool isNowOpen) {
//   if (isNowOpen && !subnodes_added_) {
//     // FIXME: make sure we do not accept changes to this tree now !
//     const THash<std::string, Object *> children = device_proxy_->children();
//     std::list<std::string>::const_iterator it = children.begin();
//     std::list<std::string>::const_iterator end = children.end();
//     for (; it != end; ++it) {
//       SimpleObjectProxy *object_proxy = TYPE_CAST(SimpleObjectProxy, *it);
//       assert(object_proxy);
//       object_proxy->sync();
//       addSubItem(object_proxy->object_proxy_view());
//     }
//     subnodes_added_ = true;
//   }
}