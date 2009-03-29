#include "oscit/receive.h"
#include "oscit/send.h"
#include "oscit/root.h"
#include "oscit/zeroconf.h"

#include "osc/OscReceivedElements.h"
#include "ip/UdpSocket.h"

namespace oscit {

OscReceive::OscReceive(Root * root, uint port) : root_(root)
{ 
  socket_ = new UdpListeningReceiveSocket( IpEndpointName( IpEndpointName::ANY_ADDRESS, port ), this );
  mRegisterZeroConf = new ZeroConfRegister("oscit", "_oscit._udp", port);
  
  std::cout << "Listening for OSC messages on port " << port << ".\n";
  listen_thread_id_ = NULL;
  pthread_create( &listen_thread_id_, NULL, &start_thread, (void*)this);
}
  
OscReceive::~OscReceive()
{
  socket_->AsynchronousBreak();
  pthread_join(listen_thread_id_, NULL);  // wait
  delete socket_;
  delete mRegisterZeroConf;
}
  
void * OscReceive::start_thread(void * pThis)
{
  ((OscReceive*)pThis)->run();
  return NULL;
}

void OscReceive::run()
{
  // Run until break.
  socket_->Run();
}
  
void OscReceive::ProcessMessage(const osc::ReceivedMessage &message, const IpEndpointName &remoteEndpoint) {
  Value res;
  std::string url(message.AddressPattern());
  
  if (url == "/.register") {
    // TODO: implement register
    // Can we avoid this exception ?
    // std::cout << "registering satellite\n";
    // root_->adopt_observer(new OscSend(remoteEndpoint));
    // return value ??
  } else {
    res = root_->call(url, value_from_osc(message));
  }
  
  // send return
  root_->send_reply(&remoteEndpoint, url, res);
}

Value OscReceive::value_from_osc(const osc::ReceivedMessage &message) {
  Value res;
  const char * type_tags = message.TypeTags();
  uint i = 0;
  osc::ReceivedMessage::const_iterator arg = message.ArgumentsBegin();
  osc::ReceivedMessage::const_iterator end = message.ArgumentsEnd();
  
  while (arg != end) {
    switch (type_tags[i]) {
      case osc::TRUE_TYPE_TAG:
        res.push_back(Value(1.0));
        break;
      case osc::FALSE_TYPE_TAG:
        res.push_back(Value(0.0));
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
} // namespace oscit