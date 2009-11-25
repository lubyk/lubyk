#include "oscit/zeroconf_browser.h"
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
  if (proxy_factory_ && command_) {
    // TODO: check that there is no proxy for this location already and
    // return true/false ?
    if (!command_->find_proxy(location)) {
      command_->adopt_proxy(proxy_factory_->build_and_init_root_proxy(location));
    }
  } else {
    fprintf(stderr, "Cannot build proxy: proxy_factory_ or command_ is NULL (%s:%i)", __FILE__, __LINE__);
  }
}

void ZeroConfBrowser::remove_proxy(const Location &location) {
  if (command_) {
    RootProxy *proxy = command_->find_proxy(location);
    if (proxy) delete proxy;
  }
}

void ZeroConfBrowser::get_protocol_from_service_type() {
  size_t dot_index = service_type_.find(".");
  if (dot_index != std::string::npos) {
    protocol_ = service_type_.substr(1, dot_index);
  } else {
    // Bad service type
    std::cerr << "Could not get protocol from service type: " << service_type_ << "\n";
  }
}

} // oscit