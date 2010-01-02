#include "mimas.h"
#include "m_proxy_factory.h"

#include "mimas_window_content.h"
#include "m_device_view.h"

ObjectProxy *MProxyFactory::build_object_proxy(Object *parent, const std::string &name, const Value &type) {
  if (parent->url() == "" && name == ".views") {
    // build views container
    return new MViewsBuilder(mimas_, name, type);
  } else if (parent->url() == "/.views" && type.type_id() == H("sss")) {
    // build view proxy
    return new MViewProxy(mimas_, name, type);
  } else if (!is_meta_method(name) || parent->url() != "") {
    return new MObjectProxy(mimas_, name, type);
  } else {
    return NULL;
  }
}