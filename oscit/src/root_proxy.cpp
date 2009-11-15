#include "oscit/root_proxy.h"
#include "oscit/command.h"
#include "oscit/object_proxy.h"
#include "oscit/proxy_factory.h"

namespace oscit {

void RootProxy::set_command(Command *command) {
  if (command_) command_->unregister_proxy(this);
  command_ = command;
  if (command_) command_->register_proxy(this);
}

void RootProxy::set_proxy_factory(ProxyFactory *factory) {
  if (proxy_factory_) proxy_factory_->unregister_proxy(this);
  proxy_factory_ = factory;
  if (proxy_factory_) proxy_factory_->register_proxy(this);
}

ObjectProxy *RootProxy::build_object_proxy(const std::string &name, const Value &type) {
  if (proxy_factory_) {
    ObjectProxy *object = proxy_factory_->build_object_proxy(name, type);
    if (object) {
      object->set_root_proxy(this);
    }
    return object;
  }
  return NULL;
}

} // oscit
