#include "oscit/osc_command.h"
#include "oscit/root.h"
#include "oscit/zeroconf.h"

#include "osc/OscReceivedElements.h"
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"

namespace oscit {

static osc::OutboundPacketStream &operator<<(osc::OutboundPacketStream &out_stream, const Value &val);

OscCommand::OscCommand(uint port) { 
  socket_ = new UdpListeningReceiveSocket( IpEndpointName( IpEndpointName::ANY_ADDRESS, port ), this );
  zeroconf_register_ = new ZeroConfRegister("oscit", "_oscit._udp", port);
}
  
OscCommand::~OscCommand() {
  printf("OscCommand::die!\n");
  quit();
  // double join ?
  delete socket_;
  delete zeroconf_register_;
}

void OscCommand::terminate(Thread *runner) {
  printf("OscCommand::terminate()!\n");
  socket_->AsynchronousBreak();
  quit_ = true;
}

void OscCommand::do_listen() {
  // Run until async break.
  socket_->Run();
}


void OscCommand::register_observer(const IpEndpointName &observer) {
  // TODO: check that the observer is not already in the list of observers_.
  observers_.push_back(observer);
}

void OscCommand::send_reply(const IpEndpointName *remote_endpoint, const std::string &url, const Value &val) {
  
  if (val.is_nil()) return;
  
  if (val.is_error()) {
    if (remote_endpoint) send(*remote_endpoint, "/.error", val);
  } else {
    // reply to all
    
    // prepare reply
    Value res(url);
    res.push_back(val);
    
    if (remote_endpoint) {
      send(*remote_endpoint, "/.reply", res);
    }
    
    send_all("/.reply", res, remote_endpoint); // skip remote_endpoint
    root_->notify_observers("/.reply", res, this);   // skip this command
  }
}
 
void OscCommand::ProcessMessage(const osc::ReceivedMessage &message, const IpEndpointName &remote_endpoint) {
  Value res;
  std::string url(message.AddressPattern());
  
  if (url == "/.register") {
    register_observer(remote_endpoint);
    // TODO: implement TTL
    // return value ??
  } else {
    Value val(value_from_osc(message));
    
    // debugging
    // char host_ip[ IpEndpointName::ADDRESS_STRING_LENGTH ];
    // // get host ip as string
    // remote_endpoint.AddressAsString(host_ip);
    // std::cout << url << " " << val << " (" << host_ip << ":" << remote_endpoint.port << ")" << std::endl;
    res = root_->call(url, val);
  }
  
  // send return
  send_reply(&remote_endpoint, url, res);
}

Value OscCommand::value_from_osc(const osc::ReceivedMessage &message) {
  Value res;
  const char * type_tags = message.TypeTags();
  uint i = 0;
  osc::ReceivedMessage::const_iterator arg = message.ArgumentsBegin();
  osc::ReceivedMessage::const_iterator end = message.ArgumentsEnd();
  
  while (arg != end) {
    switch (type_tags[i]) {
      case osc::TRUE_TYPE_TAG:
        res.push_back(1.0);
        break;
      case osc::FALSE_TYPE_TAG:
        res.push_back(0.0);
        break;
      // case osc::NIL_TYPE_TAG:
      //   ??
      // case osc::INFINITUM_TYPE_TAG:
      //   ??
      // zero length

      case osc::INT32_TYPE_TAG:
        res.push_back(Value((Real)(arg->AsInt32Unchecked())));
        break;
      case osc::FLOAT_TYPE_TAG:
        res.push_back(Value((Real)(arg->AsFloatUnchecked())));
        break;
      case osc::CHAR_TYPE_TAG:
        res.push_back(Value((Real)(arg->AsCharUnchecked())));
        break;
      case osc::DOUBLE_TYPE_TAG:
        res.push_back(Value((Real)(arg->AsDoubleUnchecked())));
        break;
      case osc::STRING_TYPE_TAG:
        res.push_back(Value(arg->AsStringUnchecked()));
        break;
      case osc::RGBA_COLOR_TYPE_TAG:
      case osc::MIDI_MESSAGE_TYPE_TAG:
      case osc::INT64_TYPE_TAG:
      case osc::TIME_TAG_TYPE_TAG:
      case osc::SYMBOL_TYPE_TAG:
      case osc::BLOB_TYPE_TAG:
      default:
        // TODO
        break;
    }
    i++;
    arg++;
  }
  return res;
}

///////////// Send osc messages /////////////////

/** Prepare message data and send it using the given socket. */
void OscCommand::send(const IpEndpointName &remote_endpoint, const char *url, const Value &val) {
  osc::OutboundPacketStream message( osc_buffer_, OSC_OUT_BUFFER_SIZE );
  build_message(url, val, &message);
  socket_->Connect(remote_endpoint);
  socket_->Send(message.Data(), message.Size());
}

/** Build osc message and send it to all observers. */
void OscCommand::send_all(const char *url, const Value &val, const IpEndpointName *skip_end_point) {
  std::list<IpEndpointName>::const_iterator it  = observers_.begin();
  std::list<IpEndpointName>::const_iterator end = observers_.end();
  
  osc::OutboundPacketStream message( osc_buffer_, OSC_OUT_BUFFER_SIZE );
  build_message(url, val, &message);
  
  while (it != end) {
    if (skip_end_point != NULL && *it == *skip_end_point) {
      // skip
    } else {
      socket_->Connect(*it++);
      socket_->Send(message.Data(), message.Size());
    }
  }
}


void OscCommand::build_message(const char *url, const Value &val, osc::OutboundPacketStream *message) {
  *message << osc::BeginBundleImmediate << osc::BeginMessage(url) << val << osc::EndMessage << osc::EndBundle;
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