#ifndef _OSCIT_SEND_H_
#define _OSCIT_SEND_H_
#include "new_value.h"
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
  
  ~OscSend();
  
  /** Send prepared message out. */
  void send (const osc::OutboundPacketStream& pMsg);
  
  /** Send url with parameter. */
  void send (const std::string& pUrl, const Value& pVal);
  
  /** Build osc message and send it to all recipients of the provided list. */
  static void send_all(std::list<OscSend*>& pRecipients, const std::string& pUrl, const char * pTypeTag, const Values pVal);
  
private:
  UdpTransmitSocket * mSocket;
  
  friend osc::OutboundPacketStream& operator<< (osc::OutboundPacketStream& os, const Value& val);
};

} // namespace oscit
#endif // _OSCIT_SEND_H_