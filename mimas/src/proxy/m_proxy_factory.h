#ifndef MIMAS_SRC_PROXY_M_PROXY_FACTORY_
#define MIMAS_SRC_PROXY_M_PROXY_FACTORY_
#include "oscit/proxy_factory.h"
#include "m_object_proxy.h"
#include "m_views_builder.h"
#include "m_view_proxy.h"

class MimasWindowContent;

class MProxyFactory : public ProxyFactory {
public:
  MProxyFactory(MimasWindowContent *mimas, Component *main_view)
      : mimas_(mimas) {}

  virtual RootProxy *build_root_proxy(const Location &end_point) {
    return new RootProxy(end_point);
  }

  virtual ObjectProxy *build_object_proxy(Object *parent, const std::string &name, const Value &type);

private:
  MimasWindowContent *mimas_;
};

#endif // MIMAS_SRC_PROXY_M_PROXY_FACTORY_