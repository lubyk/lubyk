/** Copyright 2009 Gaspard Bucher
 *
 */

#ifndef OSCIT_INCLUDE_OSCIT_PROXY_FACTORY_H_
#define OSCIT_INCLUDE_OSCIT_PROXY_FACTORY_H_

#include "oscit/root.h"

namespace oscit {

/** This class is used to bridge between oscit and a GUI when building a
 * 'ghost' tree that mirrors a remote 'real' tree.
 */
class ProxyFactory : ZeroConfBrowser {
public:
  virtual void add_device(const char *name, const char *host, unsigned int port, bool more_coming) {
    RootProxy *root_proxy = build_root_proxy(end_point);
    if (root_proxy) {
      register_proxy(root_proxy);
    }
  }

  /** Object proxy factory. This method should be overwritten in subclasses in order to
   * create custom ObjectProxy objects.
   */
  virtual ObjectProxy *build_object_proxy(const std::string &name, const Value &type) = 0;

  virtual RootProxy *build_root_proxy(Location* end_point);

private:

  void register_proxy(RootProxy* root_proxy) {
    // This enables routing 'reply' messages coming from the remote ip endpoint to
    // this root_proxy.
    command_->adopt_proxy(root_proxy);
  }

};

#endif // OSCIT_INCLUDE_OSCIT_PROXY_FACTORY_H_