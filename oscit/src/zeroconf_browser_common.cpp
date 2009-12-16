#include "oscit/zeroconf_browser.h"

#include <stdio.h>

#include <string>

#include "oscit/proxy_factory.h"
#include "oscit/root_proxy.h"
#include "oscit/object_proxy.h"
#include "oscit/command.h"

namespace oscit {

void ZeroConfBrowser::set_command(Command *command) {
  command_ = command;
}

void ZeroConfBrowser::do_adopt_proxy_factory(ProxyFactory *factory) {
  if (proxy_factory_) {
    delete proxy_factory_;
  }
  proxy_factory_ = factory;
}


void ZeroConfBrowser::add_proxy(const Location &location) {
  if (command_) {
    if (!command_->find_proxy(location)) {
      RootProxy *proxy = NULL;
      if (proxy_factory_) {
        proxy = command_->adopt_proxy(proxy_factory_->build_and_init_root_proxy(location));
      } else {
        std::cerr << "Cannot build proxy: proxy_factory_ is NULL\n";
      }
      if (proxy) added_proxy(proxy);
    }
  } else {
    std::cerr << "Cannot build proxy: command_ is NULL\n";
  }
}

void ZeroConfBrowser::remove_proxy(const Location &location) {
  if (command_) {
    RootProxy *proxy = command_->find_proxy(location);
    if (proxy) {
      proxy->detach();
      removed_proxy(proxy);
      delete proxy;
    }
  }
}

void ZeroConfBrowser::get_protocol_from_service_type() {
  size_t dot_index = service_type_.find(".");
  if (dot_index != std::string::npos) {
    protocol_ = service_type_.substr(1, dot_index - 1);
  } else {
    // Bad service type
    std::cerr << "Could not get protocol from service type: " << service_type_ << "\n";
  }
}


void ZeroConfBrowser::add_device(const Location &location) {
  // only record first entry in case there are more then one network interfaces
  if (!found_devices_.has_key(location.name())) {
    found_devices_.set(location.name(), location);
    add_proxy(location);
  }
}

void ZeroConfBrowser::remove_device(const char *name) {
  Location location;
  if (found_devices_.get(name, &location)) {
    found_devices_.remove(name);
    remove_proxy(location);
  }
}

bool ZeroConfBrowser::get_location_from_name(const char *service_name, Location *location) const {
  return found_devices_.get(service_name, location);
}


} // oscit