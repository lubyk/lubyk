#include "mimas.h"
#include "proxy_view.h"

#include <string>
#include <iostream>

#include "m_slider.h"
#include "m_pad.h"

void ProxyView::build_view(const std::string &view_defition) {
  std::cout << "BUILDING FROM\n" << view_defition << "\n";
  XmlDocument xml_document(String(view_defition.c_str()));
  XmlElement *xml_root = xml_document.getDocumentElement();
  if (!xml_root) {
    std::cerr << "Could not load xml document '" << url() << "': " << xml_document.getLastParseError().toUTF8() << "\n";
    return;
  }

  if (xml_root->getTagName() != T("view")) {
    std::cerr << "Invalid view definition '" << url() << "': root should be 'view' (found '" << xml_root->getTagName().toUTF8() << "')\n.";
    return;
  }

  MessageManagerLock mml;

  if (view_) delete view_;

  view_ = new Component;
  set_bounds_from_xml(view_, *xml_root);
  ResizableBorderComponent *resize = new ResizableBorderComponent(view_, NULL);
  resize->setBorderThickness(BorderSize(4));
  main_view_->addAndMakeVisible(view_);

  XmlElement *child = xml_root->getFirstChildElement();
  while (child != NULL) {
    String type = child->getTagName();
    if (type == T("slider")) {
      build_slider(*child);
    } else if (type == T("pad")) {
      build_pad(*child);
    }
    child = child->getNextElement();
  }
}

void ProxyView::build_slider(XmlElement &def) {
  std::cout << "build_slider\n";
  if (!check_attribute(def, "slider", T("connect"))) return;

  String connect_path = def.getStringAttribute(T("connect"));
  Value error;
  MObjectProxy *proxy = TYPE_CAST(MObjectProxy, root_->find_or_build_object_at(connect_path.toUTF8(), &error));
  if (!proxy) {
    std::cerr << "Error in '" << url() << "': could not connect to proxy " << connect_path.toUTF8() << "'\n.";
    return;
  }

  MSlider *slider = new MSlider(connect_path);
  proxy->connect(slider);

//  slider->setTextBoxStyle(Slider::TextBoxLeft, false, 80, 20);
  set_bounds_from_xml(slider, def);

  slider->setSliderStyle(slider->getWidth() > slider->getHeight() ? Slider::LinearHorizontal : Slider::LinearVertical);
  view_->addAndMakeVisible(slider);
}

void ProxyView::build_pad(XmlElement &def) {
  std::cout << "build_pad\n";
  if (!check_attribute(def, "pad", T("connect_x"))) return;
  if (!check_attribute(def, "pad", T("connect_y"))) return;

  String connect_path_x = def.getStringAttribute(T("connect_x"));
  String connect_path_y = def.getStringAttribute(T("connect_y"));

  Value error;
  MObjectProxy *proxy_x = TYPE_CAST(MObjectProxy, root_->find_or_build_object_at(connect_path_x.toUTF8(), &error));
  if (!proxy_x) {
    std::cerr << "Error in '" << url() << "': could not connect to proxy " << connect_path_x.toUTF8() << "'\n.";
    return;
  }

  MObjectProxy *proxy_y = TYPE_CAST(MObjectProxy, root_->find_or_build_object_at(connect_path_y.toUTF8(), &error));
  if (!proxy_y) {
    std::cerr << "Error in '" << url() << "': could not connect to proxy " << connect_path_y.toUTF8() << "'\n.";
    return;
  }

  MPad *pad = new MPad(connect_path_x);
  proxy_x->connect(pad->range_x());
  proxy_y->connect(pad->range_y());

  set_bounds_from_xml(pad, def);
  view_->addAndMakeVisible(pad);
}

bool ProxyView::check_attribute(XmlElement &definition, const char *type, const tchar *const name) {
  if (!definition.hasAttribute(name)) {
    std::cerr << "Error in '" << url() << "': '" << type << "' needs '" << name << "' attribute.\n.";
    return false;
  }
  return true;
}


void ProxyView::set_bounds_from_xml(Component *component, XmlElement &def) {
  double width  = def.getDoubleAttribute(T("width"));
  double height = def.getDoubleAttribute(T("height"));
  component->setBounds (def.getDoubleAttribute(T("x")),
    def.getDoubleAttribute(T("y")),
    width,
    height);
}