#include "oscit/send.h"
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"

namespace oscit {

#define OSC_OUT_BUFFER_SIZE 2048

OscSend::OscSend(const IpEndpointName& pRemoteEndpoint)
{
  mSocket = new UdpTransmitSocket( pRemoteEndpoint );
}

OscSend::OscSend(const IpEndpointName& pRemoteEndpoint, int pPort)
{ 
	mSocket = new UdpTransmitSocket( IpEndpointName( pRemoteEndpoint.address, pPort ));
  
  // debugging
  char hostIpAddress[ IpEndpointName::ADDRESS_STRING_LENGTH ];
  // get host ip as string
  pRemoteEndpoint.AddressAsString(hostIpAddress);
  std::cout << "Reply to " << hostIpAddress << ":" << (int)pParams.value() << ".\n";
}

OscSend::~OscSend()
{
  delete mSocket;
}

void OscSend::send(const osc::OutboundPacketStream& pMsg)
{
  mSocket->Send(pMsg.Data(), pMsg.Size());
}

void OscSend::send(const std::string& pUrl, const Value& pVal)
{
  char buffer[OSC_OUT_BUFFER_SIZE]; // TODO: reuse buffer
  osc::OutboundPacketStream oss( buffer, OSC_OUT_BUFFER_SIZE );
  oss << osc::BeginBundleImmediate << osc::BeginMessage(pUrl.c_str()) << pVal << osc::EndMessage << osc::EndBundle;
  send(oss);
}

void OscSend::send_all(std::list<OscSend*>& pRecipients, const std::string& pUrl, const char * pTypeTag, const Value& pVal)
{ 
  std::list<OscSend*>::iterator it  = pRecipients.begin();
  std::list<OscSend*>::iterator end = pRecipients.end();
  
  //if (it == end) return;
  // TODO: optimize: keep this thing between queries (make this not a static and let the first recipient send to all others).
  char buffer[OSC_OUT_BUFFER_SIZE]; // TODO: reuse buffer
  osc::OutboundPacketStream oss( buffer, OSC_OUT_BUFFER_SIZE );
  oss << osc::BeginBundleImmediate << osc::BeginMessage(pUrl.c_str()) << pVal << osc::EndMessage << osc::EndBundle;
  
  while (it != end) (*it++)->send(oss);
}

osc::OutboundPacketStream& operator<< (osc::OutboundPacketStream& pOut, const Value& val)
{  
  switch (val.data_type())
  {
    case BangValue:
      pOut << 1;
      break;
    case NumberValue:
      pOut << ((Number)val).value();
      break;
    case MatrixValue:
      std::cout << "MatrixValue to osc not supported yet...\n";
      break;
    case StringValue:
      pOut << ((String)val).string().c_str();
      break;
    case CharMatrixValue:
      std::cout << "MatrixValue to osc not supported yet...\n";
      break;
    case HashValue:
      pOut << ((String)val).to_string().c_str();
      break;
    case ErrorValue:
      pOut << ((Error)val).to_string().c_str();
      break;
    default:  
      std::cout << "Unknown value type ! Cannot send to osc\n";
      break;
  }
  return pOut;
}

} // namespace oscit