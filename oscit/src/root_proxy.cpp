#include "oscit/root_proxy.h"

#include <string>
#include <iostream>

#include "oscit/command.h"
#include "oscit/object_proxy.h"
#include "oscit/proxy_factory.h"

namespace oscit {

void RootProxy::set_command(Command *command) {
  if (command_) command_->unregister_proxy(this);
  command_ = command;
  if (command) {
    command->register_proxy(this);
    command->send(remote_location_, REGISTER_PATH, gNilValue);
    sync();
  }
}

void RootProxy::set_proxy_factory(ProxyFactory *factory) {
  if (proxy_factory_) proxy_factory_->unregister_proxy(this);
  proxy_factory_ = factory;
  if (proxy_factory_) proxy_factory_->register_proxy(this);
}


void RootProxy::build_children_from_types(Object *base, const Value &types) {
  if (!types.is_list()) {
    std::cerr << "Cannot handle " << LIST_WITH_TYPE_PATH << " reply: invalid argument: " << types << "\n";
    return;
  }
  if (!proxy_factory_) {
    std::cerr << "Cannot handle " << LIST_WITH_TYPE_PATH << " reply: no ProxyFactory !\n";
    return;
  }

  int types_count = types.size();
  Value name_with_type;
  ObjectProxy *object_proxy;
  bool has_children;


  for(int i=0; i < types_count; ++i) {
    name_with_type = types[i];
    if (name_with_type.size() < 2 || !name_with_type[0].is_string()) {
      std::cerr << "Invalid type in " << LIST_WITH_TYPE_PATH << " reply argument: " << name_with_type << "\n";
    } else {
      std::string name = name_with_type[0].str();
      if (name.at(name.length()-1) == '/') {
        has_children = true;
        name = name.substr(0, name.length() - 1);
      } else {
        has_children = false;
      }

      if (!base->child(name)) {
        object_proxy = proxy_factory_->build_object_proxy(base, name, name_with_type[1]);
        if (object_proxy) {
          base->adopt(object_proxy);
          object_proxy->set_need_sync(has_children);
        }
      }
    }
  }
}

Object *RootProxy::build_child(const std::string &name, Value *error) {
  if (!proxy_factory_) {
    std::cerr << "Cannot build child /" << name << " : no ProxyFactory !\n";
    return NULL;
  }
  return adopt(proxy_factory_->build_object_proxy(this, name, gNilValue));
}

void RootProxy::handle_reply(const std::string &path, const Value &val) {
  if (path == LIST_WITH_TYPE_PATH) {
    if (val.size() < 2 || !val[0].is_string() || !val[1].is_list()) {
      std::cerr << "Invalid argument in " << LIST_WITH_TYPE_PATH << " reply: " << val << "\n";
      return;
    }

    Object *base = object_at(val[0].str());
    if (!base) {
      std::cerr << "Invalid base path " << val[0].str() << " in " << LIST_WITH_TYPE_PATH << " reply: unknown path.\n";
      return;
    }

    build_children_from_types(base, val[1]);
  } else if (path == TYPE_PATH) {
    if (val.size() < 2 || !val[0].is_string()) {
      std::cerr << "Invalid argument in " << TYPE_PATH << " reply: " << val << "\n";
      return;
    }

    Value error;
    Object *target = find_or_build_object_at(val[0].str(), &error);
    ObjectProxy *object_proxy = TYPE_CAST(ObjectProxy, target);
    if (object_proxy) {
      object_proxy->set_type(val[1]);
    }
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
