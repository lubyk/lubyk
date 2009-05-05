#include <iostream>

#include <stdio.h>			// For stdout, stderr
#include <string.h>			// For strlen(), strcpy(), bzero()
#include <errno.h>      // For errno, EINTR
#include <time.h>

#ifdef _WIN32
#include <process.h>
typedef	int	pid_t;
#define	getpid	_getpid
#define	strcasecmp	_stricmp
#define snprintf _snprintf
#else
#include <sys/time.h>		// For struct timeval
#include <unistd.h>     // For getopt() and optind
#include <arpa/inet.h>	// For inet_addr()
#endif

#include <dns_sd.h>     // zeroconf

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



ZeroConfBrowser::ZeroConfBrowser(const std::string &service_type) : service_type_(service_type) {
  listen_thread_.start<ZeroConfBrowser, &ZeroConfBrowser::do_start>(this, NULL);
}

static void s_resolve_callback(AvahiServiceResolver *resolver,
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

/** Callback called on device notification.
 */
static void s_browser_callback(AvahiServiceBrowser *browser,
                               AvahiIfIndex interface,
                               AvahiProtocol protocol,
                               AvahiBrowserEvent event,
                               const char *name,
                               const char *type,
                               const char *domain,
                               AvahiLookupResultFlags flags,
                               void *context) {
  ZeroConfBrowser *zero_conf = context;
  AvahiServiceResolver *resolver;
  BrowsedDevice *device = new BrowsedDevice((ZeroConfBrowser*)context, name, domain, event);
  
  switch (event) {
    case AVAHI_BROWSER_FAILURE:
      fprintf(stderr, "Avahi browser failure (%s).\n",
                      avahi_strerror(avahi_client_errno(avahi_service_browser_get_client(b))));
      zero_conf->quit();
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
                                s_resolve_callback,  // callback
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

void ZeroConfBrowser::do_start(Thread *thread) {
  if (avahi_client_ == NULL) return;
  
  AvahiServiceBrowser *browser = NULL;
  
  
  // create browser service
  browser = avahi_service_browser_new(avahi_client_, // client
                            AVAHI_IF_UNSPEC,         // interface
                            AVAHI_PROTO_UNSPEC,      // protocol
                            service_type_.c_str(),   // service type
                            NULL,                    // domain
                            0,                       // flags
                            s_browser_callback,      // callback
                            this);                   // context
  if (browser == NULL) {
    fprintf(stderr, "Could not create avahi service browser (%s).\n", avahi_strerror(avahi_client_errno(avahi_client_)));
    return;
  }
  
  listen(thread);
  
  avahi_service_browser_free(browser);
}

} // oscit
