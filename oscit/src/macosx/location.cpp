#include <sstream>
#include "oscit/location.h"

#include <netdb.h>     // g
#include <arpa/inet.h> // inet_addr

namespace oscit {

unsigned long Location::ip_from_hostname(const char *hostname) {
  struct hostent * hostentry;
  hostentry = gethostbyname(hostname);
  if (hostentry == NULL) {
    std::cerr << "Could not resolve '" << hostname << "'.\n";
    return Location::NO_IP;
  }
  struct in_addr a;
  memcpy( &a, hostentry->h_addr_list[0], hostentry->h_length );
  unsigned long ip = ntohl(a.s_addr);
  return ip;
}

} // oscit