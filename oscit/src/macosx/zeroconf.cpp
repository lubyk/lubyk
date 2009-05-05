#include "oscit/zeroconf.h"

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

#include <iostream>

namespace oscit {

// Note: the select() implementation on Windows (Winsock2)
//       fails with any timeout much larger than this
#define LONG_TIME 100000000

ZeroConf::ZeroConf() : timeout_(LONG_TIME) {}

void ZeroConf::listen(Thread *thread, DNSServiceRef service) {
  // Run until break.
  int dns_sd_fd = DNSServiceRefSockFD(service);
  fd_set readfds;
  struct timeval tv;
  int result;

  while (thread->should_run()) {
    FD_ZERO(&readfds);
    FD_SET(dns_sd_fd, &readfds);
    tv.tv_sec = timeout_;
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
        thread->stop();
      }
    }	else if (errno != EINTR) {
      // Error.
      fprintf(stderr, "ZeroConf error (%d %s)\n", errno, strerror(errno));
      if (errno != EINTR) thread->stop();
    }
  }
}

} // oscit
