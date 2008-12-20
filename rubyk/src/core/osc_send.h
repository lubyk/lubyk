#ifndef _OSC_SEND_H_
#define _OSC_SEND_H_
#include "values.h"
#include <list>

namespace osc {
  class OutboundPacketStream;
}

class UdpTransmitSocket;
class IpEndpointName;

/** Sends osc messages through UDP. */
class OscSend
{
public:
  
  OscSend(const IpEndpointName& remoteEndpoint, const Number& pParams);
  
  ~OscSend();
  
  /** Send prepared message out. */
  void send (const osc::OutboundPacketStream& pMsg);
  
  /** Send url with parameter. */
  void send (const std::string& pUrl, const Value& pVal);
  
  /** Build osc message and send it to all recipients of the provided list. */
  static void send_all(std::list<OscSend*>& pRecipients, const std::string& pUrl, const Value& pVal);
  
private:
  UdpTransmitSocket * mSocket;
  
  friend osc::OutboundPacketStream& operator<< (osc::OutboundPacketStream& os, const Value& val);
};

#endif // _OSC_SEND_H_