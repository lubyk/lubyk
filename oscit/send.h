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
  OscSend(const IpEndpointName& pRemoteEndpoint);
  
  OscSend(const IpEndpointName& remoteEndpoint, int pPort);
  
  ~OscSend() {} // FIXME: remove {}: implementation is in send.cpp
  
  /** Send prepared message out. */
  void send (const osc::OutboundPacketStream& pMsg);
  
  /** Send url with parameter. */
  void send (const std::string &url, const Value& val);
  
  /** Build osc message and send it to all recipients of the provided list. */
  static void send_all(std::list<OscSend*>& pRecipients, const std::string &url, const Value &val);
  
private:
  UdpTransmitSocket * mSocket;
  static void values_to_stream (osc::OutboundPacketStream& pOut, TypeTagID type_tag_ids, const Value& val);
};

} // namespace oscit
#endif // _OSCIT_SEND_H_