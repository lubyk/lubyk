/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

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

#include "oscit/thread.h"
#include "oscit/zeroconf_registration.h"


namespace oscit {

static void registration_cleanup(void *data) {
  DNSServiceRef *service = (DNSServiceRef*)data;
  DNSServiceRefDeallocate(*service);
}

// Note: the select() implementation on Windows (Winsock2)
//       fails with any timeout much larger than this
#define LONG_TIME 100000000

class ZeroConfRegistration::Implementation : public Thread {
public:
  Implementation(ZeroConfRegistration *master) : master_(master) {
    start_thread<Implementation, &Implementation::registration>(this, NULL);
  }

  void register_service(DNSServiceRef service) {
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
          fprintf(stderr, "DNSServiceProcessResult returned %d\n", err);
          quit();
        }
      }	else if (errno != EINTR) {
        // Error.
        fprintf(stderr, "ZeroConf error (%d %s)\n", errno, strerror(errno));
        if (errno != EINTR) quit();
      }
    }
  }

  void registration(Thread *thread) {
    //  release calling thread semaphore
    thread->thread_ready();

    DNSServiceErrorType error;
    DNSServiceRef service;

    error = DNSServiceRegister(&service,
      0,                    // no flags
      0,                    // all network interfaces
      master_->name_.c_str(),         // name
      master_->service_type_.c_str(), // service type
      "",                   // register in default domain(s)
      NULL,                 // use default host name
      htons(master_->port_),// port number
      0,                    // length of TXT record
      NULL,                 // no TXT record
      Implementation::register_callback,  // callback function
      (void*)master_);         // context

    if (error == kDNSServiceErr_NoError) {
      pthread_cleanup_push(registration_cleanup, &service);
        register_service(service);
      pthread_cleanup_pop(0); // 0 = do not execute on pop
    } else {
      fprintf(stderr,"Could not register service %s.%s on port %u (error %d)\n", master_->name_.c_str(), master_->service_type_.c_str(), master_->port_, error);//, strerror(errno));
    }

    DNSServiceRefDeallocate(service);
  }

  /** Callback called after registration. */
  static void register_callback(DNSServiceRef ref,
                                DNSServiceFlags flags,
                                DNSServiceErrorType error,
                                const char *name,
                                const char *service_type,
                                const char *host,
                                void *context) {

    if (error != kDNSServiceErr_NoError) {
      fprintf(stderr, "DNSServiceRegister error (%d).\n", error);
    } else {
      ZeroConfRegistration *reg = (ZeroConfRegistration*)context;
      reg->name_ = name;
      reg->host_ = host;
      reg->registration_done();
    }
  }
  ZeroConfRegistration *master_;
};


ZeroConfRegistration::ZeroConfRegistration(const std::string &name, const char *service_type, uint16_t port) : name_(name), service_type_(service_type), port_(port) {
  impl_ = new ZeroConfRegistration::Implementation(this);
}

ZeroConfRegistration::~ZeroConfRegistration() {
  stop();
  delete impl_;
}

void ZeroConfRegistration::stop() {
  impl_->kill();
}

} // oscit
