#include "oscit/send.h"
#include "oscit/base_object.h"
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"

namespace oscit {

#define OSC_OUT_BUFFER_SIZE 2048

OscSend::OscSend(const IpEndpointName &remote_endpoint) {
  remote_endpoint_ = new IpEndpointName(remote_endpoint);
  socket_ = new UdpTransmitSocket(*remote_endpoint_);
}

OscSend::OscSend(const IpEndpointName &remote_endpoint, int port) { 
  remote_endpoint_ = new IpEndpointName(remote_endpoint.address, port);
	socket_ = new UdpTransmitSocket(*remote_endpoint_);
  
  // debugging
  char hostIpAddress[ IpEndpointName::ADDRESS_STRING_LENGTH ];
  // get host ip as string
  remote_endpoint.AddressAsString(hostIpAddress);
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

void OscSend::send(const char *url, const Value &val) {
  send(socket_, url, val);
}

void OscSend::send(const std::string &url, const Value &val) {
  send(socket_, url.c_str(), val);
}

///////// class methods /////////


/** Prepare message data and send it using the given socket. */
void OscSend::send(UdpSocket *socket, const IpEndpointName &remote_endpoint, const char *url, const Value &val) {
  char buffer[OSC_OUT_BUFFER_SIZE]; // TODO: reuse buffer
  osc::OutboundPacketStream message( buffer, OSC_OUT_BUFFER_SIZE );
  build_message(url, val, &message);
  socket->Connect(remote_endpoint);
  socket->Send(message.Data(), message.Size());
}

void OscSend::send(UdpSocket *socket, const IpEndpointName &remote_endpoint, osc::OutboundPacketStream &message) {
  socket->Connect(remote_endpoint);
  socket->Send(message.Data(), message.Size());
}
  
/** Prepare message data and send it using the given socket. */
void OscSend::send(UdpSocket *socket, const char *url, const Value &val) {
  char buffer[OSC_OUT_BUFFER_SIZE]; // TODO: reuse buffer
  osc::OutboundPacketStream message( buffer, OSC_OUT_BUFFER_SIZE );
  build_message(url, val, &message);
  
  socket->Send(message.Data(), message.Size());
}

/** Build osc message and send it to all recipients of the provided list. */
void OscSend::send_all(UdpSocket *socket, const std::list<IpEndpointName> &recipients, const char *url, const Value &val) {
  std::list<IpEndpointName>::const_iterator it  = recipients.begin();
  std::list<IpEndpointName>::const_iterator end = recipients.end();
  
  char buffer[OSC_OUT_BUFFER_SIZE]; // TODO: reuse buffer
  osc::OutboundPacketStream message( buffer, OSC_OUT_BUFFER_SIZE );
  build_message(url, val, &message);
  
  while (it != end) {
    socket->Connect(*it++);
    socket->Send(message.Data(), message.Size());
  }
}


void OscSend::build_message(const char *url, const Value &val, osc::OutboundPacketStream *message) {
  *message << osc::BeginBundleImmediate << osc::BeginMessage(url) << val << osc::EndMessage << osc::EndBundle;
}

/** Prepare message data and send it using the given socket. */
void OscSend::send(UdpSocket *socket, osc::OutboundPacketStream &message) {
  socket->Send(message.Data(), message.Size());
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
      out_stream << val.c_str();
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