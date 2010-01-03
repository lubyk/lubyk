#include "mimas.h"
#include "m_proxy_factory.h"

#include "mimas_window_content.h"
#include "m_device_view.h"

ObjectProxy *MProxyFactory::build_object_proxy(Object *parent, const std::string &name, const Value &type) {
  std::cout << "BUILD: " << name << " : " << type.type_tag() << "\n";
  if (parent->url() == "" && name == Url(VIEWS_PATH).name()) {
    // build views container
    return new MViewsBuilder(mimas_, name, type);
  } else if (parent->url() == VIEWS_PATH && type.type_id() == H("Hs")) {
    // build view proxy
    // TODO: move this as build_child in MViewsBuilder.
    return new MViewProxy(mimas_, name, type);
  } else if (!is_meta_method(name) || parent->url() != "") {
    return new MObjectProxy(mimas_, name, type);
  } else {
    return NULL;
  }
}