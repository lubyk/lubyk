#ifndef OSCIT_INCLUDE_OSCIT_LOCATION_H_
#define OSCIT_INCLUDE_OSCIT_LOCATION_H_
#include <string>
#include <ostream>

#include "oscit/thash.h"

namespace oscit {

#define DEFAULT_PROTOCOL "oscit"

class ZeroConfBrowser;

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

  Location() : reference_by_hostname_(false), ip_(NO_IP), port_(NO_PORT) {}
  Location(const char *protocol, const char *service_name) :
                      protocol_(protocol), name_(service_name),
                      reference_by_hostname_(false), ip_(NO_IP), port_(NO_PORT) {}
  Location(const char *protocol, const char *service_name, const char *hostname, uint port) :
                      protocol_(protocol), name_(service_name),
                      reference_by_hostname_(false), ip_(NO_IP), port_(port){
    ip_ = ip_from_hostname(hostname);
  }
  Location(const char *protocol, const char *hostname, uint port) :
                      protocol_(protocol), name_(hostname),
                      reference_by_hostname_(true), ip_(NO_IP), port_(port) {}
  Location(const char *protocol, unsigned long ip, uint port) :
                      protocol_(protocol),
                      reference_by_hostname_(true), ip_(ip), port_(port) {
    name_ = name_from_ip(ip_);
  }
  Location(const char *service_name) :
                      protocol_(DEFAULT_PROTOCOL), name_(service_name),
                      reference_by_hostname_(false), ip_(NO_IP), port_(NO_PORT) {}
  Location(const char *hostname, uint port) :
                      protocol_(DEFAULT_PROTOCOL), name_(hostname),
                      reference_by_hostname_(true), ip_(NO_IP), port_(port) {}
  Location(unsigned long ip, uint port) :
                      protocol_(DEFAULT_PROTOCOL),
                      reference_by_hostname_(true), ip_(ip), port_(port) {
    name_ = name_from_ip(ip_);
  }

  void clear() {
    protocol_ = "";
    name_ = "";
    reference_by_hostname_ = false;
    ip_   = NO_IP;
    port_ = NO_PORT;
  }

  bool operator!=(const Location &other) const {
    return ip_       != other.ip_       ||
           port_     != other.port_     ||
           protocol_ != other.protocol_ ||
           reference_by_hostname_ != other.reference_by_hostname_ ||
           name_     != other.name_;
  }

  bool operator==(const Location &other) const {
    return protocol_ == other.protocol_ &&
           (
             (ip_ != Location::NO_IP && ip_ == other.ip_ && port_ == other.port_) ||
             (reference_by_hostname_ == other.reference_by_hostname_ && name_ == other.name_)
           );
  }

  const std::string &name() const {
    return name_;
  }

  const std::string &protocol() const {
    return protocol_;
  }

  const unsigned long &ip() const {
    return ip_;
  }

  const uint &port() const {
    return port_;
  }

  void set_port(uint port) {
    port_ = port;
  }

  /** Used for debugging: show location with ip and port.
   */
  const std::string inspect() const;

  void resolve_with(const ZeroConfBrowser *browser);

  static unsigned long ip_from_hostname(const char *hostname);
  static const std::string name_from_ip(unsigned long ip);

private:
  friend std::ostream &operator<<(std::ostream &out_stream, const Location &location);
  friend class Url;
  friend uint hashId(const Location &location);


  /** Protocol used (oscit, http, ...)
   */
  std::string protocol_;

  /** This can contain either a hostname (example.com) or a service
   *  name ("stage camera").
   */
  std::string name_;

  /** This tells us if the name_ contains a hostname or a
   *  service name (true if it is a hostname).
   */
  bool reference_by_hostname_;

  /** Once the location has been resolved, we have an IP.
   */
  unsigned long ip_;

  /** Port of the remote object (set when location is resolved)
   */
  uint port_;
};

inline uint hashId(const Location &location) {
  if (location.ip_ != Location::NO_IP) {
    return hashId(location.ip_) + hashId(location.port_);
  } else {
    // FIXME: hashid name_, protocol_, etc.....
    return hashId(location.name_);
  }
}

std::ostream &operator<< (std::ostream &out_stream, const std::list<Location> &locations);


} // oscit

#endif // OSCIT_INCLUDE_OSCIT_LOCATION_H_