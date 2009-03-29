#include "oscit/send.h"
#include "oscit/object.h"
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"

namespace oscit {

#define OSC_OUT_BUFFER_SIZE 2048

OscSend::OscSend(const IpEndpointName &remoteEndpoint) {
  remote_endpoint_ = new IpEndpointName(remoteEndpoint);
  socket_ = new UdpTransmitSocket(*remote_endpoint_);
}

OscSend::OscSend(const IpEndpointName &remoteEndpoint, int port) { 
  remote_endpoint_ = new IpEndpointName(remoteEndpoint.address, port);
	socket_ = new UdpTransmitSocket(*remote_endpoint_);
  
  // debugging
  char hostIpAddress[ IpEndpointName::ADDRESS_STRING_LENGTH ];
  // get host ip as string
  remoteEndpoint.AddressAsString(hostIpAddress);
  std::cout << "Reply to " << hostIpAddress << ":" << port << ".\n";
}

OscSend::OscSend(const char *remote, int port) {
  remote_endpoint_ = new IpEndpointName(remote, port);
  socket_ = new UdpTransmitSocket(*remote_endpoint_); 
}

OscSend::~OscSend()
{
  delete socket_;
  delete remote_endpoint_;
}

void OscSend::send(const osc::OutboundPacketStream &message)
{
  socket_->Send(message.Data(), message.Size());
}

void OscSend::send(const char *url, const Value& val)
{
  char buffer[OSC_OUT_BUFFER_SIZE]; // TODO: reuse buffer
  osc::OutboundPacketStream oss( buffer, OSC_OUT_BUFFER_SIZE );
  oss << osc::BeginBundleImmediate << osc::BeginMessage(url) << val << osc::EndMessage << osc::EndBundle;
  
  
  // debugging
  char host_ip[ IpEndpointName::ADDRESS_STRING_LENGTH ];
  // get host ip as string
  remote_endpoint_->AddressAsString(host_ip);
  std::cout << "[" << host_ip << ":" << remote_endpoint_->port << "] << " << val << std::endl;

  send(oss);
}

void OscSend::send_all(std::list<OscSend*> &recipients, const char *url, const Value& val)
{ 
  std::list<OscSend*>::iterator it  = recipients.begin();
  std::list<OscSend*>::iterator end = recipients.end();
  
  //if (it == end) return;
  // TODO: optimize: keep this thing between queries (make this not a static and let the first recipient send to all others).
  char buffer[OSC_OUT_BUFFER_SIZE]; // TODO: reuse buffer
  osc::OutboundPacketStream oss( buffer, OSC_OUT_BUFFER_SIZE );
  oss << osc::BeginBundleImmediate << osc::BeginMessage(url) << val << osc::EndMessage << osc::EndBundle;
  
  while (it != end) (*it++)->send(oss);
}

osc::OutboundPacketStream &operator<<(osc::OutboundPacketStream &out_stream, const Value &val) {
  switch (val.type()) {
    case REAL_VALUE:
      out_stream << val.r;
      break;
    case ERROR_VALUE:
      out_stream << val.error_code() << val.error_message().c_str();
      break;
    case STRING_VALUE:
      out_stream << val.s;
      break;
    case NIL_VALUE:
      out_stream << osc::Nil;
      break;
    case LIST_VALUE:
      size_t sz = val.size();
      for (size_t i = 0; i < sz; ++i) {
        out_stream << val[i];
      }
      break;
    default:
      ;// ????
  }
  return out_stream;
}


} // namespace oscit