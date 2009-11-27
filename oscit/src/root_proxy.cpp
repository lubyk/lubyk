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
    command_->send(remote_location_, REGISTER_PATH, gNilValue);
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
  Value path_with_type;
  ObjectProxy *object_proxy;
  bool has_children;


  for(int i=0; i < types_count; ++i) {
    path_with_type = types[i];
    if (path_with_type.size() < 2 || !path_with_type[0].is_string()) {
      std::cerr << "Invalid type in " << LIST_WITH_TYPE_PATH << " reply argument: " << path_with_type << "\n";
    } else {
      std::string path = path_with_type[0].str();
      if (path.size() < 1 || path.at(0) == '.') {
        // meta method, ignore ?
      } else {
        if (path.at(path.length()-1) == '/') {
          has_children = true;
          path = path.substr(0, path.length() - 1);
        } else {
          has_children = false;
        }
        if (!base->child(path)) {
          object_proxy = base->adopt(proxy_factory_->build_object_proxy(path, path_with_type[1]));
          if (object_proxy) object_proxy->set_need_sync(has_children);
        }
      }
    }
  }
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
