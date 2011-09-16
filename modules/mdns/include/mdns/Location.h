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

#ifndef LUBYK_INCLUDE_MDNS_LOCATION_H_
#define LUBYK_INCLUDE_MDNS_LOCATION_H_
#include <string>
#include <ostream>

//#include "oscit/thash.h"
typedef unsigned int uint;

namespace mdns {

#define DEFAULT_PROTOCOL "lubyk"

class ZeroConfBrowser;

/** Browse for devices matching a give service type. Call a lua function
 * when devices are added or removed.
 *
 * @dub string_format:'%%s'
 *      string_args:'(*userdata)->inspect().c_str()'
 */
class Location
{
public:
  /** IP not configured yet.
   */
  static const unsigned long NO_IP = 0;
  /** Any local IP.
   */
  static const unsigned long ANY_IP = 0xFFFFFFFF;
  static const unsigned long LOOPBACK = (127<<24) + 1; // 127.0.0.1
  static const uint NO_PORT = 0;
  static const uint DEFAULT_INTERFACE = 0;

  Location(const char *protocol,
           const char *service_name,
           const char *hostname,
           uint port = NO_PORT,
           uint interface = DEFAULT_INTERFACE)
      : protocol_(protocol),
        name_(service_name),
        host_(hostname),
        ip_(ip_from_hostname(hostname)),
        port_(port),
        interface_(interface) {}

  Location(const char *protocol,
           const char *hostname,
           uint port)
      : protocol_(protocol),
        host_(hostname),
        ip_(ip_from_hostname(hostname)),
        port_(port) {}

  Location(const char *protocol,
           unsigned long ip,
           uint port)
      : protocol_(protocol),
        host_(name_from_ip(ip_)),
        ip_(ip),
        port_(port) {}

  Location(const char *protocol,
           const char *service_name)
      : protocol_(protocol),
        name_(service_name),
        ip_(NO_IP),
        port_(NO_PORT) {}

  Location()
      : ip_(NO_IP),
        port_(NO_PORT) {}

  void clear() {
    protocol_ = "";
    name_ = "";
    host_ = "";
    ip_   = NO_IP;
    port_ = NO_PORT;
  }

  bool operator!=(const Location &other) const {
    return !(this->operator==(other));
  }

  bool operator==(const Location &other) const {
    return protocol_ == other.protocol_ &&
           (
             (ip_ != Location::NO_IP && ip_ == other.ip_ && port_ == other.port_) ||
             (host_ == other.host_ && port_ == other.port_)
           );
  }

  const char *protocol() const {
    return protocol_.c_str();
  }

  const char *name() const {
    return name_.c_str();
  }

  const char *host() const {
    return host_.c_str();
  }

  const unsigned long &ip() const {
    return ip_;
  }

  const uint &port() const {
    return port_;
  }

  const uint &interface() const {
    return interface_;
  }

  void set_port(uint port) {
    port_ = port;
  }

  /** Used for debugging: show location with ip and port.
   */
  const std::string inspect() const;

  static unsigned long ip_from_hostname(const char *hostname);
  static const std::string name_from_ip(unsigned long ip);

private:
  friend std::ostream &operator<<(std::ostream &out_stream, const Location &location);
  friend class Url;
  friend uint hashId(const Location &location);


  /** Protocol used (oscit, http, ...)
   */
  std::string protocol_;

  /** This contains the service's name ("stage camera").
   */
  std::string name_;

  /** This is the host's name ('gaspard.local', 'example.com').
   */
  std::string host_;

  /** Once the location has been resolved, we have an IP.
   */
  unsigned long ip_;

  /** Port of the remote object (set when location is resolved)
   */
  uint port_;

  /** Interface used to connect to this location (set by mdns Browser).
   */
  uint interface_;
};

std::ostream &operator<<(std::ostream &out_stream, const Location &location);

} // mdns

#endif // LUBYK_INCLUDE_MDNS_LOCATION_H_
