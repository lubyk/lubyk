#include <sstream>
#include "oscit/location.h"

#include <netdb.h>     // gethostbyname
#include <arpa/inet.h> // inet_addr

namespace oscit {

unsigned long Location::ip_from_hostname(const char *hostname) {
  struct hostent * hostentry;
  hostentry = gethostbyname(hostname);
  if (hostentry == NULL) {
    std::cerr << "Could not resolve '" << hostname << "'.\n";
    return Location::NO_IP;
  }
  unsigned long ip = inet_addr(hostentry->h_addr_list[0]);
  return ip;
}

} // oscit