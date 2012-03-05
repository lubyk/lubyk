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

#include "lk/SelectCallback.h"
#include "mdns/Browser.h"
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

#include <queue>

namespace mdns {

class Browser::Implementation {
  Browser *master_;
  DNSServiceRef service_;

  /** Last detected services.
   */
  std::queue<Service*> found_services_;
  int fd_;
  bool query_next_;
public:
  Implementation(Browser *master)
      : master_(master)
      , query_next_(true)
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
    while (!found_services_.empty()) {
      delete found_services_.front();
      found_services_.pop();
    }

    DNSServiceRefDeallocate(service_);
  }

  int fd() {
    return fd_;
  }

  // [2] Return found services. If this returns NULL, the calling app must
  // waitRead on fd() before asking again.
  Service *getService() {
    if (query_next_) {
      // calls sBrowseCallback
      DNSServiceErrorType err = DNSServiceProcessResult(service_);
      if (err) {
        // An error occured. Halt.
        fprintf(stderr, "DNSServiceProcessResult error (%d).\n", err);
        return 0;
      }
      query_next_ = false;
    }

    if (found_services_.empty()) {
      query_next_ = true;
      return NULL;
    }

    Service *service = found_services_.front();
    found_services_.pop();
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
    impl->found_services_.push(new Service(
        impl->master_->service_type_,
        name,
        interface_index,
        type,
        domain,
        flags & kDNSServiceFlagsAdd
        ));
  }

};

Browser::Browser(Context *ctx, const char *service_type)
      : service_type_(service_type) {
  setProtocolFromServiceType();
  impl_ = new Implementation(this);
  fd_ = impl_->fd();
}

Browser::~Browser() {
  delete impl_;
}

LuaStackSize Browser::getService(lua_State *L) {
  Service *service = impl_->getService();

  if (service) {
    // Service is a dub::Thread. We must push it with proper initialization
    // on the stack. GC by Lua.  
    service->pushobject(L, service, "mdns.Service", true);
    return 1;
  } else {
    return 0;
  }
}

} // mdns



