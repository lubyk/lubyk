#ifndef MIMAS_SRC_CORE_BROWSER_PROXY_FACTORY_
#define MIMAS_SRC_CORE_BROWSER_PROXY_FACTORY_
#include "oscit/proxy_factory.h"
#include "device_view.h"

class BrowserProxyFactory : public ProxyFactory {
public:
  virtual RootProxy *build_root_proxy(const Location &end_point) {
    return new DeviceProxy(end_point);
  }

  virtual ObjectProxy *build_object_proxy(const std::string &name, const Value &type) {
    return NULL;
  }

};

#endif // MIMAS_SRC_CORE_BROWSER_PROXY_FACTORY_