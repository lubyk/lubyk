#include "oscit/zeroconf_browser.h"
#include "oscit/thread.h"
#include "oscit/proxy_factory.h"
#include "oscit/root_proxy.h"
#include "oscit/object_proxy.h"

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
#include <sys/time.h>		// struct timeval
#include <unistd.h>     // getopt() and optind ?
#include <arpa/inet.h>	// inet_addr() ?
#endif

#include <dns_sd.h>     // zeroconf

namespace oscit {

// Note: the select() implementation on Windows (Winsock2)
//       fails with any timeout much larger than this
#define LONG_TIME 100000000

struct BrowsedDevice {
  BrowsedDevice(ZeroConfBrowser *browser, const char *name, const char *host, DNSServiceFlags flags) :
                name_(name), host_(host), browser_(browser), flags_(flags) {}
  std::string name_;
  std::string host_;
  ZeroConfBrowser *browser_;
  DNSServiceFlags flags_;
};

class ZeroConfBrowser::Implementation : public Thread {
public:
  Implementation(ZeroConfBrowser *master) : master_(master) {
    start_thread<Implementation, &Implementation::browse>(this, NULL);
  }

  void browse(Thread *thread) {
    //  release calling thread semaphore
    thread_ready();

    DNSServiceErrorType error;
    DNSServiceRef       service;


    error = DNSServiceBrowse(&service,
      0,                     // no flags
      0,                     // all network interfaces
      master_->service_type_.c_str(), // service type
      NULL,                  // default domain(s)
      Implementation::browser_callback,    // callback function
      (void*)master_);       // context

    if (error == kDNSServiceErr_NoError) {
      browse(service);
    } else {
      fprintf(stderr,"Could not browse for service %s (error %d)\n", master_->service_type_.c_str(), error);//, strerror(errno));
    }

    DNSServiceRefDeallocate(service);
  }

  void browse(DNSServiceRef service) {
    // Run until break.
    int dns_sd_fd = DNSServiceRefSockFD(service);
    fd_set readfds;
    struct timeval tv;
    int result;

    while (should_run()) {
      FD_ZERO(&readfds);
      FD_SET(dns_sd_fd, &readfds);
      tv.tv_sec = LONG_TIME;
      tv.tv_usec = 0;
                      // highest fd in set + 1
      result = select(dns_sd_fd+1, &readfds, (fd_set*)NULL, (fd_set*)NULL, &tv);
      if (result > 0) {
        DNSServiceErrorType err = kDNSServiceErr_NoError;
        // Execute callback
        if (FD_ISSET(dns_sd_fd, &readfds)) err = DNSServiceProcessResult(service);

        if (err) {
          // An error occured. Halt.
          fprintf(stderr, "DNSServiceProcessResult error (%d).\n", err);
          quit();
        }
      }	else if (errno != EINTR) {
        // Error.
        fprintf(stderr, "ZeroConf error (%d %s)\n", errno, strerror(errno));
        if (errno != EINTR) quit();
      }
    }
  }

  static void resolve_callback(DNSServiceRef service,
                               DNSServiceFlags flags,
                               uint32_t interface_index,
                               DNSServiceErrorType error,
                               const char *fullname,
                               const char *hostname,
                               uint16_t port,
                               uint16_t txt_len,
                               const unsigned char *txt,
                               void *context) {
    
    BrowsedDevice *device = (BrowsedDevice*)context;
    
    if (device->flags_ & kDNSServiceFlagsAdd) {
      device->browser_->add_device(Location(
                               device->browser_->protocol_.c_str(),
                               device->name_.c_str(),
                               hostname,
                               ntohs(port)
                               ));
    } else {
      device->browser_->remove_device(device->name_.c_str());
    }
  }

  static void browser_callback(DNSServiceRef service,
                               DNSServiceFlags flags,
                               uint32_t interface_index,
                               DNSServiceErrorType error,
                               const char *name,
                               const char *type,
                               const char *domain,
                               void *context) {

    if (error != kDNSServiceErr_NoError) {
      fprintf(stderr, "DNSServiceBrowse error (%d).\n", error);
      return;
    }

    DNSServiceRef resolve_service;
    BrowsedDevice *device = new BrowsedDevice((ZeroConfBrowser*)context, name, domain, flags);
    error = DNSServiceResolve(&resolve_service,
               0,    // flags
               interface_index,
               name,
               type,
               domain,
               Implementation::resolve_callback,
               (void*)device);  // context

    if (error == kDNSServiceErr_NoError) {
      error = DNSServiceProcessResult(resolve_service);
      if (error != kDNSServiceErr_NoError) {
        fprintf(stderr, "DNSServiceProcessResult failed: %i\n", error);
      }
    } else {
      fprintf(stderr,"Error while trying to resolve %s @ %s (%d)\n", name, domain, error);
    }

    DNSServiceRefDeallocate(resolve_service);
    delete device;
  }

  ZeroConfBrowser *master_;
};

ZeroConfBrowser::ZeroConfBrowser(const char *service_type) :
                  service_type_(service_type),
                  command_(NULL),
                  proxy_factory_(NULL),
                  found_devices_(FOUND_DEVICE_HASH_SIZE) {
  get_protocol_from_service_type();
  impl_ = new ZeroConfBrowser::Implementation(this);
}

ZeroConfBrowser::~ZeroConfBrowser() {
  delete impl_;
  if (proxy_factory_) delete proxy_factory_;
}

void ZeroConfBrowser::stop() {
  impl_->kill();
}

} // oscit



