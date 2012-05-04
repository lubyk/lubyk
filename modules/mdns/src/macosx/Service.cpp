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

namespace mdns {

class Service::Implementation {
  Service *master_;
  DNSServiceRef service_;
  int fd_;
public:
  Implementation(Service *master, int interface_index, const char *name, const char *type, const char *domain)
    : master_(master)
  {
    DNSServiceErrorType error = DNSServiceResolve(&service_,
        0,    // flags
        interface_index,
        name,
        type,
        domain,
        Implementation::sResolve,
        (void*)this);  // context
    if (error != kDNSServiceErr_NoError) {
      fprintf(stderr, "DNSServiceResolve error (%i).\n", error);
      fd_ = 0;
    } else {
      fd_ = DNSServiceRefSockFD(service_);
    }
  }

  ~Implementation() {
    DNSServiceRefDeallocate(service_);
  }

  int fd() {
    return fd_;
  }

  // Will call sResolve -> Service::set
  bool getInfo() {
    DNSServiceErrorType error = DNSServiceProcessResult(service_);
    if (error != kDNSServiceErr_NoError) {
      fprintf(stderr, "Could not process DNS resolution (%i).\n", error);
      return false;
    }
    // Service::set called.
    return true;
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
    Implementation *impl = (Implementation*)context;
    impl->master_->set(
        hostname,
        ntohs(port),
        std::string((const char *)txt, txt_len)
    );
  }
};



Service::Service(std::string service_type, const char *name, int interface_index, const char *type, const char *domain, bool is_add)
  : service_type_(service_type)
  , name_(name)
  , is_add_(is_add)
{
  impl_ = new Implementation(this, interface_index, name, type, domain);
  fd_ = impl_->fd();
}

Service::~Service() {
  delete impl_;
}

bool Service::getInfo() {
  return impl_->getInfo();
}

} // mdns




