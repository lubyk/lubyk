/*
  ==============================================================================

   This file is part of the LUBYK library (http://lubyk.org)
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

#include "mdns/Location.h"

#include "lubyk/lua.h"

#include <sstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h> // inet_ntop

#include <iostream>
/*

struct addrinfo {
  int ai_flags;             // input flags
  int ai_family;            // protocol family for socket
  int ai_socktype;          // socket type
  int ai_protocol;          // protocol for socket
  socklen_t ai_addrlen;     // length of socket-address
  struct sockaddr *ai_addr; // socket-address for socket
  char *ai_canonname;       // canonical name for service location
  struct addrinfo *ai_next; // pointer to next in list
};

// For other structs related to this topic: http://beej.us/guide/bgnet/output/html/multipage/sockaddr_inman.html

*/
namespace mdns {

unsigned long Location::ip_from_hostname(const char *hostname) {
  struct addrinfo hints, *res, *result_list;
  unsigned long ip = 0;
  int error;

  memset(&hints, 0, sizeof(hints));
  error = getaddrinfo(hostname, NULL, &hints, &result_list);
  if (error) {
    std::cerr << "Could not resolve '" << hostname << "' (" << gai_strerror(error) << ")\n";
    return Location::NO_IP;
  }

  for (res = result_list; res; res = res->ai_next) {
    char addrstr[100];
    inet_ntop (res->ai_family, res->ai_addr->sa_data, addrstr, 100);

    if (res->ai_family == AF_INET) {
      // only support AF_INET for now (no IPv6)

      // sockaddr is a generic raw bunch of bytes, we must cast it first to
      // an IPv4 struct:
      struct sockaddr_in *ipv4 = (sockaddr_in*) res->ai_addr;

      // ip is in sin_addr
      // we must convert the 32-bit IP from network to host
      ip = ntohl(ipv4->sin_addr.s_addr);

      // we do not break so that we display the last IPv4 address because
      // mac os X automatically assigns 172.16.xxx.xxx addresses and they
      // come first and we prefer showing the address assigned with DHCP.
      // break;
    }
  }

  freeaddrinfo(result_list);
  return ip;
}

} // mdns
