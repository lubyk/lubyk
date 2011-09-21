/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
   Copyright (c) 2007-2011 by Gaspard Bucher (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#include "mdns/AbstractBrowser.h"
#include "lubyk/thread.h"

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

using namespace lubyk;

namespace mdns {

// Note: the select() implementation on Windows (Winsock2)
//       fails with any timeout much larger than 100000000
#define LONG_TIME 5

struct BrowsedDevice {
  BrowsedDevice(AbstractBrowser *browser, const char *name, const char *host, DNSServiceFlags flags) :
                name_(name), host_(host), master_(browser), flags_(flags) {}
  std::string name_;
  std::string host_;
  AbstractBrowser *master_;
  DNSServiceFlags flags_;
};

class AbstractBrowser::Implementation : public Thread {
  DNSServiceRef service_;
public:
  Implementation(AbstractBrowser *master)
      : master_(master) {
    getFiledescriptor();
  }

  ~Implementation() {
    DNSServiceRefDeallocate(service_);
  }

  void getFiledescriptor() {
    DNSServiceErrorType error;

    error = DNSServiceBrowse(&service_,
      0,                     // no flags
      0,                     // all network interfaces
      master_->service_type_.c_str(), // service type
      NULL,                  // default domain(s)
      sGetServiceInfo,       // callback function
      (void*)master_);       // context

    if (error == kDNSServiceErr_NoError) {
      master_->fd_ = DNSServiceRefSockFD(service_);
    } else {
      fprintf(stderr,"Could not browse for service %s (error %d)\n", master_->service_type_.c_str(), error);//, strerror(errno));
    }
  }

  bool getServices() {
    // calls getServiceInfo
    DNSServiceErrorType err = DNSServiceProcessResult(service_);
    if (err) {
      // An error occured. Halt.
      fprintf(stderr, "DNSServiceProcessResult error (%d).\n", err);
      return false;
    }
    return true;
  }

  static void sGetServiceInfo(DNSServiceRef service,
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
    BrowsedDevice *device = new BrowsedDevice((AbstractBrowser*)context, name, domain, flags);
    error = DNSServiceResolve(&resolve_service,
               0,    // flags
               interface_index,
               name,
               type,
               domain,
               Implementation::sResolve,
               (void*)device);  // context

    sResolveSelect(resolve_service, device);

    DNSServiceRefDeallocate(resolve_service);
    delete device;
  }

  void browse() {
    // Run until break.
    int dns_sd_fd = DNSServiceRefSockFD(service_);
    fd_set readfds;
    struct timeval tv;
    int result;

    while (true) {
      FD_ZERO(&readfds);
      FD_SET(dns_sd_fd, &readfds);
      tv.tv_sec = LONG_TIME;
      tv.tv_usec = 0;
                      // highest fd in set + 1
      result = select(dns_sd_fd+1, &readfds, (fd_set*)NULL, (fd_set*)NULL, &tv);
      if (result > 0) {
        DNSServiceErrorType err = kDNSServiceErr_NoError;
        // Execute callback
        if (FD_ISSET(dns_sd_fd, &readfds)) err = DNSServiceProcessResult(service_);

        if (err) {
          // An error occured. Halt.
          fprintf(stderr, "DNSServiceProcessResult error (%d).\n", err);
          break;
        }
      }	else if (errno != EINTR) {
        // Error.
        fprintf(stderr, "ZeroConf error (%d %s)\n", errno, strerror(errno));
        if (errno != EINTR) break;
      }
    }
  }
  
  static void sResolveSelect(DNSServiceRef service, BrowsedDevice *device) {
    int dns_sd_fd = DNSServiceRefSockFD(service);
    fd_set readfds;
    struct timeval tv;
    int result;

    while (true) {
      FD_ZERO(&readfds);
      FD_SET(dns_sd_fd, &readfds);
      tv.tv_sec = LONG_TIME;
      tv.tv_usec = 0;
                      // highest fd in set + 1
      result = select(dns_sd_fd+1, &readfds, (fd_set*)NULL, (fd_set*)NULL, &tv);
      if (result > 0) {
        DNSServiceErrorType error = kDNSServiceErr_NoError;
        // Execute callback
        if (FD_ISSET(dns_sd_fd, &readfds)) {
          error = DNSServiceProcessResult(service);
          if (error != kDNSServiceErr_NoError) {
            fprintf(stderr, "DNSServiceProcessResult failed: %i\n", error);
          }
          break;
        }
      }	else if (errno != EINTR) {
        // Error.
        fprintf(stderr, "ZeroConf error (%d %s)\n", errno, strerror(errno));
        break;
      }
    }
  }

  static void sResolve(DNSServiceRef service,
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

    //printf("%s %i (%s)\n", device->name_.c_str(), ntohs(port), device->flags_ & kDNSServiceFlagsAdd ? "ADD" : "REMOVE");
    if (device->flags_ & kDNSServiceFlagsAdd) {
      port = ntohs(port);
    } else {
      port = 0;
    }
    device->master_->found_services_.push(Location(
                             device->master_->protocol_.c_str(),
                             device->name_.c_str(),
                             hostname,
                             port,
                             interface_index
                             ));
  }

  AbstractBrowser *master_;
};

AbstractBrowser::AbstractBrowser(const char *service_type)
      : service_type_(service_type) {
  setProtocolFromServiceType();
  impl_ = new AbstractBrowser::Implementation(this);
}

AbstractBrowser::~AbstractBrowser() {
  delete impl_;
}

bool AbstractBrowser::getServices() {
  return impl_->getServices();
}

} // mdns



