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

#include <iostream>

#include <dns_sd.h>     // zeroconf

#include "mdns/AbstractRegistration.h"


namespace mdns {

class AbstractRegistration::Implementation {
  DNSServiceRef service_;
public:
  Implementation(AbstractRegistration *master) : master_(master) {
    getFiledescriptor();
  }

  ~Implementation() {
    DNSServiceRefDeallocate(service_);
  }

  void getFiledescriptor() {
    size_t txt_len = master_->txt_.size();
    DNSServiceErrorType error;
    error = DNSServiceRegister(&service_,
      0,                    // no flags
      0,                    // all network interfaces
      master_->name_.c_str(),         // name
      master_->service_type_.c_str(), // service type
      "",                   // register in default domain(s)
      NULL,                 // use default host name
      htons(master_->port_),// port number
      txt_len,              // length of TXT record
      txt_len ? master_->txt_.c_str() : NULL,  // TXT record
      sGetServiceInfo,      // callback function
      (void*)master_);      // context

    if (error == kDNSServiceErr_NoError) {
      master_->fd_ = DNSServiceRefSockFD(service_);
    } else {
      master_->fd_ = 0;
      fprintf(stderr,"Could not register service %s.%s on port %u (error %d)\n", master_->name_.c_str(), master_->service_type_.c_str(), master_->port_, error);//, strerror(errno));
    }

  }

  bool getService() {
    DNSServiceErrorType err = DNSServiceProcessResult(service_);
    if (err) {
      // An error occured. Halt.
      fprintf(stderr, "DNSServiceProcessResult returned %d\n", err);
      return false;
    }
    return true;
  }

  /** Callback called after registration. */
  static void sGetServiceInfo(DNSServiceRef ref,
                               DNSServiceFlags flags,
                               DNSServiceErrorType error,
                               const char *name,
                               const char *service_type,
                               const char *host,
                               void *context) {

    if (error != kDNSServiceErr_NoError) {
      fprintf(stderr, "DNSServiceRegister error (%d).\n", error);
    } else {
      AbstractRegistration *reg = (AbstractRegistration*)context;
      reg->name_ = name;
      reg->host_ = host;
    }
  }
  AbstractRegistration *master_;
};


AbstractRegistration::AbstractRegistration(const char *service_type, const char *name, uint port, const char *txt)
    : name_(name), service_type_(service_type), port_(port), txt_(txt), fd_(0) {
  impl_ = new AbstractRegistration::Implementation(this);
}

AbstractRegistration::~AbstractRegistration() {
  delete impl_;
}

bool AbstractRegistration::getService() {
  return impl_->getService();
}

} // mdns
