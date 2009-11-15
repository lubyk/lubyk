#include "oscit/proxy_factory.h"
#include "oscit/root_proxy.h"

namespace oscit {

RootProxy *ProxyFactory::build_and_init_root_proxy(const Location &end_point) {
  RootProxy *root_proxy = build_root_proxy(end_point);
  if (root_proxy) {
    root_proxy->set_proxy_factory(this);
  }
  return root_proxy;
}

void ProxyFactory::register_proxy(RootProxy *proxy) {
  root_proxies_.remove(proxy);
  root_proxies_.push_back(proxy);
}

void ProxyFactory::unregister_proxy(RootProxy *proxy) {
  root_proxies_.remove(proxy);
}

} // oscit