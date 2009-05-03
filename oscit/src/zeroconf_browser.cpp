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
  BrowsedDevice(ZeroConfBrowser *browser, const char *name, const char *host, DNSServiceFlags flags) :
                name_(name), host_(host), browser_(browser), flags_(flags) {}
  std::string name_;              
  std::string host_;
  ZeroConfBrowser *browser_;
  DNSServiceFlags flags_;
};

ZeroConfBrowser::ZeroConfBrowser(const std::string &service_type) : service_type_(service_type) {
  listen_thread_.start<ZeroConfBrowser, &ZeroConfBrowser::do_start>(this, NULL);
}

static void s_resolve_callback(DNSServiceRef service, DNSServiceFlags flags, uint32_t interface_index,
                               DNSServiceErrorType error, const char *fullname, const char *host_target,
                               uint16_t port, uint16_t txt_len, const unsigned char *txt, void *context) {
  BrowsedDevice *device = (BrowsedDevice*)context;
  
  //printf("flags:%i\n interface_index:%i\n error:%i\n fullname:%s\n host_target:%s\n port:%u\n txt_len:%i\n", flags, interface_index, error, fullname, host_target, ntohs(port), txt_len);
  if (device->flags_ & kDNSServiceFlagsAdd) {
    device->browser_->add_device(device->name_, device->host_, ntohs(port), device->flags_ & kDNSServiceFlagsMoreComing);
  } else {
    device->browser_->remove_device(device->name_, device->host_, ntohs(port), device->flags_ & kDNSServiceFlagsMoreComing);
  }
  
  DNSServiceRefDeallocate(service);
  delete device;
}

/** Callback called on device notification.
 */
static void s_browser_callback(DNSServiceRef service, DNSServiceFlags flags, uint32_t interface_index,
                               DNSServiceErrorType error, const char *name, const char *type,
                               const char *domain, void *context) {
  if (error != kDNSServiceErr_NoError) {
    fprintf(stderr, "browser_callback returned error %d.\n", error);
  } else {
    DNSServiceRef service;
    BrowsedDevice *device = new BrowsedDevice((ZeroConfBrowser*)context, name, domain, flags);
    error = DNSServiceResolve(&service,
               0,    // flags
               interface_index,
               name,
               type,
               domain,
               s_resolve_callback,
               (void*)device);  // context

    if (error == kDNSServiceErr_NoError) {
     error = DNSServiceProcessResult(service);
     if (error != kDNSServiceErr_NoError) {
       fprintf(stderr, "DNSServiceProcessResult failed: %i\n", error);
       DNSServiceRefDeallocate(service);
       delete device;
     }
    } else {  
     DNSServiceRefDeallocate(service);
     fprintf(stderr,"Error while trying to resolve %s @ %s (%d)\n", name, domain, error);
    }
  }
}

void ZeroConfBrowser::do_start(Thread *thread) {
  DNSServiceErrorType error;
  DNSServiceRef       service;

  error = DNSServiceBrowse(&service,
    0,                     // no flags
    0,                     // all network interfaces
    service_type_.c_str(), // service type
    NULL,                  // default domain(s)
    s_browser_callback,    // callback function
    (void*)this);          // context

  if (error == kDNSServiceErr_NoError) {
    listen(thread, service);
  } else {
    fprintf(stderr,"Could not browse for service %s (error %d)\n", service_type_.c_str(), error);//, strerror(errno));
  }
  DNSServiceRefDeallocate(service);
}

} // namespace oscit