#ifndef _OSCIT_SEND_H_
#define _OSCIT_SEND_H_
#include "values.h"
#include <list>

namespace osc {
  class OutboundPacketStream;
}

class UdpTransmitSocket;
class IpEndpointName;

namespace oscit {

/** Sends osc messages through UDP. */
class OscSend
{
public:
  OscSend(const IpEndpointName &remote_endpoint);
  
  OscSend(const IpEndpointName &remote_endpoint, int port);

  OscSend(const char *remote, int port);
  
  ~OscSend();
  
  /** Send prepared message out. */
  void send(const osc::OutboundPacketStream &message);
  
  /** Send a Value to the given url. */
  void send(const char *url, const Value &val);
  
  /** Send url with parameter. */
  void send(const std::string &url, const Value &val) {
    send(url.c_str(), val);
  }
  
  const IpEndpointName &remote_endpoint() {
    return *remote_endpoint_;
  }
  
  /** Build osc message and send it to all recipients of the provided list. */
  static void send_all(std::list<OscSend*> &recipients, const char *url, const Value &val);
  
  /** Build osc message and send it to all recipients of the provided list. */
  static void send_all(std::list<OscSend*> &recipients, std::string &url, const Value &val) {
    send_all(recipients, url.c_str(), val);
  }
  
 private:
  UdpTransmitSocket * socket_;
  IpEndpointName    * remote_endpoint_;
};

osc::OutboundPacketStream &operator<<(osc::OutboundPacketStream &out_stream, const Value &val);
} // namespace oscit
#endif // _OSCIT_SEND_H_