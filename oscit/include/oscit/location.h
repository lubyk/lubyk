#ifndef OSCIT_INCLUDE_OSCIT_LOCATION_H_
#define OSCIT_INCLUDE_OSCIT_LOCATION_H_
#include <string>
#include <ostream>

typedef unsigned int uint;

namespace oscit {

/** A location is an abstract reference to a place that is not in the current tree. It can be a remote host
 *  refered to by ip address and port, a pipe or any other place.
 */
class Location
{
public:
  Location(const std::string &protocol, unsigned long ip, uint port) : protocol_(protocol), ip_(ip), port_(port) {}
  Location(const char        *protocol, unsigned long ip, uint port) : protocol_(protocol), ip_(ip), port_(port) {}

  const std::string &protocol() {
    return protocol_;
  }

  const std::string &host() {
    return host_;
  }

  const unsigned long &ip() {
    return ip_;
  }

  const uint &port() {
    return port_;
  }

private:
  /** Protocol used to reach the remote location: this indicates to the dispatcher (Root) which
   *  command it should use (OscCommand for 'osc', PipeCommand for 'pipe', etc).
   */
  std::string protocol_;

  /** Remote host's name (not used when we have an IP)
   */
  std::string host_;

  /** IP address when the location is a remote host reached through the network.
   */
  unsigned long ip_;

  /** Host port used when the location is a remote host reached through the network.
   */
  uint        port_;
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_LOCATION_H_