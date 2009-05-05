#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <string>

#include <avahi-client/client.h>
#include <avahi-client/publish.h>

#include <avahi-common/alternative.h>
#include <avahi-common/simple-watch.h>
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>
#include <avahi-common/timeval.h>

#include "oscit/zeroconf.h"

namespace oscit {

struct BrowsedDevice {
  BrowsedDevice(ZeroConfBrowser *browser, const char *name, const char *host, AvahiBrowserEvent event) :
                name_(name), host_(host), browser_(browser), event_(event) {}
  std::string name_;              
  std::string host_;
  ZeroConfBrowser *browser_;
  AvahiBrowserEvent event_;
};

class ZeroConfBrowser::Implementation {
public:
  Implementation(ZeroConfBrowser *master) : master_(master) : avahi_poll_(NULL), avahi_client_(NULL) {
    do_start();
  }
  
  ~Implementation() {
    quit();
    if (avahi_poll_) avahi_simple_poll_free(avahi_poll_);
    if (avahi_client_) avahi_client_free(avahi_client_);
  }
  
  void do_start() {
    int error;
    // create poll object
    avahi_poll_ = avahi_avahi_poll_new();
    if (avahi_poll_ == NULL) {
      fprintf(stderr, "Could not create avahi simple poll object.\n");
      return;
    }

    // create client
    avahi_client_ = avahi_client_new(avahi_avahi_poll_get(avahi_poll_),
                              0,                 // flags
                              Implementation::client_callback, // callback
                              this,              // context
                              &error);

    if (avahi_client_ == NULL) {
      fprintf(stderr, "Failed to create avahi client (%s).\n", avahi_strerror(error));
      return;
    }
    
    if (avahi_client_ == NULL) return;
  
    AvahiServiceBrowser *browser = NULL;
  
  
    // create browser service
    browser = avahi_service_browser_new(avahi_client_, // client
                              AVAHI_IF_UNSPEC,         // interface
                              AVAHI_PROTO_UNSPEC,      // protocol
                              service_type_.c_str(),   // service type
                              NULL,                    // domain
                              0,                       // flags
                              Implementation::browser_callback,      // callback
                              this);                   // context
    if (browser == NULL) {
      fprintf(stderr, "Could not create avahi service browser (%s).\n", avahi_strerror(avahi_client_errno(avahi_client_)));
      return;
    }
  
    listen(thread);
  
    avahi_service_browser_free(browser);
  }
  
  void quit() {
    avahi_simple_poll_quit(avahi_poll_);
  }
  
  static void client_callback(AvahiClient *client, AvahiClientState state, void *context) {
    Implementation *impl = (Implementation*)context;

    if (state == AVAHI_CLIENT_FAILURE) {
      fprintf(stderr, "Server connection failure: %s\n", avahi_strerror(avahi_client_errno(client)));
      impl->quit();
    }
  }
  
  
  /** Callback called on device notification.
   */
  static void browser_callback(AvahiServiceBrowser *browser,
                                 AvahiIfIndex interface,
                                 AvahiProtocol protocol,
                                 AvahiBrowserEvent event,
                                 const char *name,
                                 const char *type,
                                 const char *domain,
                                 AvahiLookupResultFlags flags,
                                 void *context) {
    Implementation *impl = (Implementation*)context;
    AvahiServiceResolver *resolver;
    BrowsedDevice *device = new BrowsedDevice(impl->master_, name, domain, event);

    switch (event) {
      case AVAHI_BROWSER_FAILURE:
        fprintf(stderr, "Avahi browser failure (%s).\n",
                        avahi_strerror(avahi_client_errno(impl->avahi_client_)));
        impl->quit();
        break;
      case AVAHI_BROWSER_NEW: /* continue */  
      case AVAHI_BROWSER_REMOVE:
        resolver = avahi_service_resolver_new(zero_conf->avahi_client(), // client
                                  interface,           // interface
                                  protocol,            // 
                                  name,                // 
                                  type,                // 
                                  domain,              // 
                                  AVAHI_PROTO_UNSPEC,  // address protocol (IPv4, IPv6)
                                  0,                   // flags
                                  Implementation::resolve_callback,  // callback
                                  device);             // context
        if (resolver == NULL) {
          fprintf(stderr,"Error while trying to resolve %s @ %s (%s)\n", name, domain, avahi_strerror(avahi_client_errno(c)));
        } else {
          avahi_service_resolver_free(resolver);
        }
        break;
      case AVAHI_BROWSER_ALL_FOR_NOW:
      case AVAHI_BROWSER_CACHE_EXHAUSTED:
        fprintf(stderr, "(Browser) %s\n", event == AVAHI_BROWSER_CACHE_EXHAUSTED ? "CACHE_EXHAUSTED" : "ALL_FOR_NOW");
        break;
    }

    delete device;
  }
  
  
  static void resolve_callback(AvahiServiceResolver *resolver,
                               AVAHI_GCC_UNUSED AvahiIfIndex interface,
                               AVAHI_GCC_UNUSED AvahiProtocol protocol,
                               AvahiResolverEvent event,
                               const char *name,
                               const char *type,
                               const char *domain,
                               const char *host_name,
                               const AvahiAddress *address,
                               uint16_t port,
                               AvahiStringList *txt,
                               AvahiLookupResultFlags flags,
                               void* context) {

    BrowsedDevice *device = (BrowsedDevice*)context;
    switch (event) {
     case AVAHI_RESOLVER_FAILURE:
        fprintf(stderr, "Error while trying to resolve %s @ %s (%d)\n", name, domain,
                        avahi_strerror(avahi_client_errno(avahi_service_resolver_get_client(r))));
        break;
      case AVAHI_RESOLVER_FOUND:
        if (device->event_ == AVAHI_BROWSER_NEW) {
          device->browser_->add_device(name, domain, port, false); // more coming ?
        } else {
          device->browser_->remove_device(name, domain, port, false);
        }
        break;
      }
    }
  }
  
  ZeroConfBrowser *master_;
  AvahiSimplePoll *avahi_poll_;
  AvahiClient     *avahi_client_;
};

ZeroConfBrowser::ZeroConfBrowser(const std::string &service_type) : service_type_(service_type) {
  new ZeroConfBrowser::Implementation(this);
}

ZeroConfBrowser::~ZeroConfBrowser() {
  delete impl_;
}

} // oscit
