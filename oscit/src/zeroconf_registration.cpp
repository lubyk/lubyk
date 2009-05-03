#include "oscit/zeroconf.h"

#include <dns_sd.h>     // zeroconf
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

namespace oscit {

/** Callback called after registration. */
static void s_register_callback(DNSServiceRef ref, DNSServiceFlags flags, DNSServiceErrorType error, const char *name,
                             const char *service_type, const char *domain, void * context) {
  
  if (error != kDNSServiceErr_NoError) {
   fprintf(stderr, "register_callback returned error %d.\n", error);
  } else {
    ((ZeroConfRegistration*)context)->registration_done(name);
  }
}

void ZeroConfRegistration::start() {
  listen_thread_.start<ZeroConfRegistration, &ZeroConfRegistration::do_start>(this, NULL);
}

void ZeroConfRegistration::do_start(Thread *thread) {
  DNSServiceErrorType error;
  DNSServiceRef       serviceRef;

  error = DNSServiceRegister(&serviceRef,
    0,                    // no flags
    0,                    // all network interfaces
    name_.c_str(),        // name
    service_type_.c_str(), // service type
    "",                   // register in default domain(s)
    NULL,                 // use default host name
    htons(port_),         // port number
    0,                    // length of TXT record
    NULL,                 // no TXT record
    s_register_callback,  // callback function
    (void*)this);         // context

  if (error == kDNSServiceErr_NoError) {
    listen(thread, serviceRef);
  } else {
    fprintf(stderr,"Could not register service %s.%s on port %u (error %d)\n", name_.c_str(), service_type_.c_str(), port_, errno);//, strerror(errno));
  }
  
  DNSServiceRefDeallocate(serviceRef);
}

} // namespace oscit