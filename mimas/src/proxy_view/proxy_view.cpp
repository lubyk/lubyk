#include "mimas.h"
#include "proxy_view.h"

#include <string>
#include <iostream>

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

  XmlElement *child = xml_root->getFirstChildElement();
  while (child != NULL) {
    String type = child->getTagName();
    if (type == T("slider")) {
      build_slider(*child);
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
  MessageManagerLock mml;
  ObservableSlider *slider = proxy->build_slider();
//  slider->setTextBoxStyle(Slider::TextBoxLeft, false, 80, 20);
  double width  = def.getDoubleAttribute(T("width"));
  double height = def.getDoubleAttribute(T("height"));
  slider->setBounds (def.getDoubleAttribute(T("x")),
                     def.getDoubleAttribute(T("y")),
                     width,
                     height);
  slider->setSliderStyle(width > height ? Slider::LinearHorizontal : Slider::LinearVertical);
  main_view_->addAndMakeVisible(slider);
}

bool ProxyView::check_attribute(XmlElement &definition, const char *type, const tchar *const name) {
  if (!definition.hasAttribute(name)) {
    std::cerr << "Error in '" << url() << "': '" << type << "' needs '" << name << "' attribute.\n.";
    return false;
  }
  return true;
}