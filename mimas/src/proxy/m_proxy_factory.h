#ifndef MIMAS_SRC_PROXY_M_PROXY_FACTORY_
#define MIMAS_SRC_PROXY_M_PROXY_FACTORY_
#include "oscit/proxy_factory.h"
#include "m_root_proxy.h"
#include "m_object_proxy.h"
#include "m_views_builder.h"
#include "m_view_proxy.h"

class MimasWindowContent;

class MProxyFactory : public ProxyFactory {
public:
  MProxyFactory(MimasWindowContent *mimas, Component *main_view)
      : mimas_(mimas) {}

  virtual RootProxy *build_root_proxy(const Location &end_point) {
    return new MRootProxy(end_point);
  }

  virtual ObjectProxy *build_object_proxy(Object *parent, const std::string &name, const Value &type) {
    if (parent->url() == "" && name == ".views") {
      return new MViewsBuilder(mimas_, name, type);
    } else if (parent->url() == "/.views" && type.type_id() == H("sss")) {
      // build view
      return new MViewProxy(mimas_, name, type);
    } else if (!is_meta_method(name) || parent->url() != "") {
      return new MObjectProxy(mimas_, name, type);
    } else {
      return NULL;
    }
  }
private:
  MimasWindowContent *mimas_;
};

#endif // MIMAS_SRC_PROXY_M_PROXY_FACTORY_