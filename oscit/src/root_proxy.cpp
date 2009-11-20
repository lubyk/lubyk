#include "oscit/root_proxy.h"
#include "oscit/command.h"
#include "oscit/object_proxy.h"
#include "oscit/proxy_factory.h"

namespace oscit {

void RootProxy::set_command(Command *command) {
  if (command_) command_->unregister_proxy(this);
  command_ = command;
  if (command_) {
    command_->register_proxy(this);
    sync();
  }
}

void RootProxy::set_proxy_factory(ProxyFactory *factory) {
  if (proxy_factory_) proxy_factory_->unregister_proxy(this);
  proxy_factory_ = factory;
  if (proxy_factory_) proxy_factory_->register_proxy(this);
}


void RootProxy::handle_list_with_type_reply(ObjectProxy *target, const Value &children) {
  if (!children.is_list()) return;
  int max = children.size();
  Object *child;
  Value child_def;

  if (!proxy_factory_) {
    std::cerr << "Cannot handle replies: no ProxyFactory !\n";
    return;
  }

  for(int i = 0; i < max; ++i) {
    child_def = children[i];
    // TODO: make sure child_def is of a proper format !
    child = target->child(child_def[0].str());
    if (!child) {
      target->adopt(proxy_factory_->build_object_proxy(child_def[0].str(), child_def[1]));
    } else {
      // ignore
      // set type ?
    }
  }
}

void RootProxy::handle_reply(const std::string &path, const Value &val) {
  if (path == LIST_WITH_TYPE_PATH) {
    // FIXME: ...
  } else {
    // Find target
    Object *target = object_at(path);

    if (target && target->can_receive(val)) {
      ObjectProxy *object_proxy = TYPE_CAST(ObjectProxy, target);
      // TODO: Should we use 'safe' here ? (mutex)
      if (object_proxy) object_proxy->handle_value_change(val);
    } else {
      std::cerr << "could not route '" << path << "' reply to '" << val << "'\n";
    }
  }
}

} // oscit
