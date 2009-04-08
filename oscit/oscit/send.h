#ifndef _OSCIT_SEND_H_
#define _OSCIT_SEND_H_
#include "oscit/values.h"
#include <list>

namespace osc {
  class OutboundPacketStream;
}

class UdpTransmitSocket;
class IpEndpointName;
class UdpSocket;

namespace oscit {

/** Sends osc messages through UDP. */
class OscSend
{
public:
  OscSend(const IpEndpointName &remote_endpoint);
  
  OscSend(const IpEndpointName &remote_endpoint, int port);

  OscSend(const char *remote, int port);
  
  ~OscSend();
  
  /** Send a Value to the given url. */
  void send(const char *url, const Value &val);
  
  /** Send url with parameter. */
  void send(const std::string &url, const Value &val);
  
  const IpEndpointName &remote_endpoint() {
    return *remote_endpoint_;
  }
  
  /** Prepare message data and send it using the given socket. */
  static void send(UdpSocket *socket, const IpEndpointName &remote_endpoint, const char *url, const Value &val);
  
  /** Prepare message data and send it using the given socket. */
  static void send(UdpSocket *socket, const IpEndpointName &remote_endpoint, osc::OutboundPacketStream &message);
  
  /** Prepare message data and send it using the given socket. */
  static void send(UdpSocket *socket, const char *url, const Value &val);
  
  /** Build osc message and send it to all recipients of the provided list. */
  static void send_all(UdpSocket *socket, const std::list<IpEndpointName> &recipients, const char *url, const Value &val);
  
  /** Build osc message and send it to all recipients of the provided list. */
  static void send_all(UdpSocket *socket, const std::list<IpEndpointName> &recipients, std::string &url, const Value &val) {
    send_all(socket, recipients, url.c_str(), val);
  }
  
  static void build_message(const char *url, const Value &val, osc::OutboundPacketStream *message);
  
  /** Prepare message data and send it using the given socket. */
  static void send(UdpSocket *socket, osc::OutboundPacketStream &message);
  
 private:
  UdpTransmitSocket * socket_;
  IpEndpointName    * remote_endpoint_;
};

osc::OutboundPacketStream &operator<<(osc::OutboundPacketStream &out_stream, const Value &val);
} // namespace oscit
#endif // _OSCIT_SEND_H_