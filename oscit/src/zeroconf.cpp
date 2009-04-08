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
#include <unistd.h>         // For getopt() and optind
#include <arpa/inet.h>		// For inet_addr()
#endif

#include <iostream>

namespace oscit {

// Note: the select() implementation on Windows (Winsock2)
//fails with any timeout much larger than this
#define LONG_TIME 100000000

ZeroConf::ZeroConf() : quit_(false), timeout_(LONG_TIME)
{ 
  listen_thread_id_ = NULL;
  pthread_create( &listen_thread_id_, NULL, &start_thread, (void*)this);
}
  
ZeroConf::~ZeroConf()
{
  quit_ = true;
  pthread_kill(listen_thread_id_, SIGTERM);
  pthread_join(listen_thread_id_, NULL);  // wait
}
  
void * ZeroConf::start_thread(void * pThis)
{
  ((ZeroConf*)pThis)->start();
  return NULL;
}

void ZeroConf::listen(DNSServiceRef service_ref)
{
  // Run until break.
  int dns_sd_fd = DNSServiceRefSockFD(service_ref);
  fd_set readfds;
  struct timeval tv;
  int result;

  while (!quit_)
  {
    FD_ZERO(&readfds);
    FD_SET(dns_sd_fd, &readfds);
    tv.tv_sec = timeout_;
    tv.tv_usec = 0;
                    // highest fd in set + 1
    result = select(dns_sd_fd+1, &readfds, (fd_set*)NULL, (fd_set*)NULL, &tv);
    if (result > 0)
    {
      DNSServiceErrorType err = kDNSServiceErr_NoError;
      // Execute callback
      if (FD_ISSET(dns_sd_fd, &readfds)) err = DNSServiceProcessResult(service_ref);
      
      if (err) {
        // An error occured. Halt.
        fprintf(stderr, "DNSServiceProcessResult returned %d\n", err);
        quit_ = true;
      }
    }	else if (errno != EINTR) {
      // Error.
      fprintf(stderr, "ZeroConf error (%d %s)\n", errno, strerror(errno));
      if (errno != EINTR) quit_ = true;
    }
  }
}

/** Callback called after registration. */
static void registerCallBack(DNSServiceRef pRef, DNSServiceFlags pFlags, DNSServiceErrorType error, const char * name, const char * service_type, const char * pDomain, void * pContext)
{
  ((ZeroConfRegister*)pContext)->register_callback(error, name, service_type, pDomain);
}

void ZeroConfRegister::register_callback(DNSServiceErrorType error, const char * name, const char * service_type, const char * pDomain)
{
  if (error != kDNSServiceErr_NoError)
    fprintf(stderr, "registerCallBack returned error %d.\n", error);
  else {
    // Registration succeeded.
    name_ = name; // in case name clash
    fprintf(stdout,"Registration ok for %s.%s\n", name_.c_str(), service_type_.c_str());
  }
}

void ZeroConfRegister::start()
{
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
    registerCallBack,     // call back function
    (void*)this);         // context

  if (error == kDNSServiceErr_NoError)
    listen(serviceRef);
  else
    fprintf(stderr,"Could not register service %s.%s on port %u (error %d)\n", name_.c_str(), service_type_.c_str(), port_, errno);//, strerror(errno));
  
  
  DNSServiceRefDeallocate(serviceRef);
}
} // namespace oscit