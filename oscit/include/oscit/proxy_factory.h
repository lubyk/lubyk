/** Copyright 2009 Gaspard Bucher
 *
 */

#ifndef OSCIT_INCLUDE_OSCIT_PROXY_FACTORY_H_
#define OSCIT_INCLUDE_OSCIT_PROXY_FACTORY_H_

#include "oscit/root.h"
#include "oscit/root_proxy.h"
#include "oscit/object_proxy.h"

namespace oscit {

class ObjectProxy;
class RootProxy;

/** This class is used to bridge between oscit and a GUI when building a
 * 'ghost' tree that mirrors a remote 'real' tree.
 */
class ProxyFactory
{
public:
  ProxyFactory() {}

  virtual ~ProxyFactory() {}

  /** Called by ZeroConfBrowser to create a new RootProxy and pass
   *  it this object as factory.
   */
  RootProxy *build_and_init_root_proxy(const Location &location);

  /** Root proxy factory. This method should be overwritten in subclasses in order to
   *  create custom RootProxy objects. This is called by build_and_init_root_proxy.
   */
  virtual RootProxy *build_root_proxy(const Location &location) {
    return new RootProxy(location);
  }

  /** Object proxy factory. This method should be overwritten in subclasses in order to
   * create custom ObjectProxy objects.
   */
  virtual ObjectProxy *build_object_proxy(const std::string &name, const Value &type) {
    return new ObjectProxy(name, type);
  }

  /** A RootProxy is using this object as factory, keep a link in case it is removed.
   */
  void register_proxy(RootProxy *proxy);

  /** Forget about this root proxy.
   */
  void unregister_proxy(RootProxy *proxy);

 private:

  /** List of RootProxies that use this factory. The list simply serves to remove the dependency
   *  when the ProxyFactory is deleted.
   */
  std::list<RootProxy *> root_proxies_;
};

} // oscit
#endif // OSCIT_INCLUDE_OSCIT_PROXY_FACTORY_H_