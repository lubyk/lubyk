#ifndef MIMAS_SRC_PROXY_M_PROXY_FACTORY_
#define MIMAS_SRC_PROXY_M_PROXY_FACTORY_
#include "oscit/proxy_factory.h"
#include "m_root_proxy.h"
#include "m_object_proxy.h"
#include "m_views_builder.h"
#include "m_view_proxy.h"

class MProxyFactory : public ProxyFactory {
public:
  MProxyFactory(Component *main_view) : workspace_(main_view) {}

  virtual RootProxy *build_root_proxy(const Location &end_point) {
    return new MRootProxy(end_point);
  }

  virtual ObjectProxy *build_object_proxy(Object *parent, const std::string &name, const Value &type) {
    if (parent->url() == "" && name == ".views") {
      return new MViewsBuilder(name, type);
    } else if (parent->url() == "/.views" && type.type_id() == H("sss")) {
      // build view
      return new MViewProxy(name, type, workspace_);
    } else if (!is_meta_method(name) || parent->url() != "") {
      return new MObjectProxy(name, type);
    } else {
      return NULL;
    }
  }
private:
  Component *workspace_;
};

#endif // MIMAS_SRC_PROXY_M_PROXY_FACTORY_