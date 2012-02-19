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
#include "mdns/Service.h"

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

struct BrowsedDevice {
  BrowsedDevice(AbstractBrowser *browser, const char *name, const char *host, DNSServiceFlags flags) :
                name_(name), host_(host), master_(browser), flags_(flags) {}
  std::string name_;
  std::string host_;
  AbstractBrowser *master_;
  DNSServiceFlags flags_;
};

class AbstractBrowser::Implementation : public Thread {
  AbstractBrowser *master_;
  DNSServiceRef service_;
  Service *found_service_;
  int fd_;
public:
  Implementation(AbstractBrowser *master)
      : master_(master)
      , found_service_(0)
  {
    DNSServiceErrorType error;

    error = DNSServiceBrowse(&service_,
      0,                     // no flags
      0,                     // all network interfaces
      master_->service_type_.c_str(), // service type
      NULL,                  // default domain(s)
      sBrowseCallback,       // callback function
      (void*)this);          // context

    if (error == kDNSServiceErr_NoError) {
      fd_ = DNSServiceRefSockFD(service_);
    } else {
      fd_ = 0;
      fprintf(stderr,"Could not browse for service %s (error %d)\n", master_->service_type_.c_str(), error);//, strerror(errno));
    }
  }

  ~Implementation() {
    if (found_service_) {
      delete found_service_;
      found_service_ = NULL;
    }

    DNSServiceRefDeallocate(service_);
  }

  int fd() {
    return fd_;
  }

  // [2] found a service, return a new mdns::Service
  Service *getService() {
    // calls sBrowseCallback
    DNSServiceErrorType err = DNSServiceProcessResult(service_);
    if (err) {
      // An error occured. Halt.
      fprintf(stderr, "DNSServiceProcessResult error (%d).\n", err);
      return 0;
    }
    Service *service = found_service_;
    found_service_ = NULL;
    return service;
  }

  // [2.1] found device callback
  static void sBrowseCallback(DNSServiceRef service,
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
    Implementation *impl = (Implementation*)context;
    impl->found_service_ = new Service(
        impl->master_->service_type_,
        name,
        interface_index,
        type,
        domain,
        flags & kDNSServiceFlagsAdd
        );
  }

};

AbstractBrowser::AbstractBrowser(const char *service_type)
      : service_type_(service_type) {
  setProtocolFromServiceType();
  impl_ = new AbstractBrowser::Implementation(this);
  fd_ = impl_->fd();
}

AbstractBrowser::~AbstractBrowser() {
  delete impl_;
}

Service *AbstractBrowser::getService() {
  return impl_->getService();
}

} // mdns



