#include "oscit/receive.h"
#include "oscit/send.h"
#include "oscit/root.h"
#include "oscit/zeroconf.h"

#include "osc/OscReceivedElements.h"
#include "ip/UdpSocket.h"

namespace oscit {

OscReceive::OscReceive(Root * pRoot, uint pPort) : mRoot(pRoot)
{ 
  mSocket = new UdpListeningReceiveSocket( IpEndpointName( IpEndpointName::ANY_ADDRESS, pPort ), this );
  mRegisterZeroConf = new ZeroConfRegister("oscit", "_oscit._udp", pPort);
  
  std::cout << "Listening for OSC messages on port " << pPort << ".\n";
  mListenThreadId = NULL;
  pthread_create( &mListenThreadId, NULL, &start_thread, (void*)this);
}
  
OscReceive::~OscReceive()
{
  mSocket->AsynchronousBreak();
  pthread_join(mListenThreadId, NULL);  // wait
  delete mSocket;
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
  mSocket->Run();
}
  
void OscReceive::ProcessMessage( const osc::ReceivedMessage& pMsg, const IpEndpointName& remoteEndpoint )
{
  std::string url(pMsg.AddressPattern());
  Object * obj;
  Value res;
  char types[MAX_TYPE_TAGS_LENGTH + 1];
  const char * retTypes = types;
  uint typeTags = casted_type_tags(types, pMsg.TypeTags());
  
  std::cout << "\n>> " << url << " " << types << "\n> ";
  
  if (url == "/.reply_to" && typeTags == REAL_SIGNATURE) {
    // Can we avoid this exception ?
    std::cout << "registering satellite\n";
    Value param;
    values_from_osc(&param, pMsg);
    mRoot->reply_to(new OscSend(remoteEndpoint, (int)param.f));
    // ??? res = 
  } else if (mRoot->get(&obj, url)) {  
    // The code here is a speedup to avoid allocation of params.
    if (typeTags == obj->mTypeTag) {
      Value param = obj->mParam;
      values_from_osc(&param, pMsg);
      res = obj->trigger(&param);
    } else {
      // invalid, use NULL
      res = obj->trigger(NULL);
      // return type is now
      retTypes = obj->mTypeTagString.c_str();
    }
  } else {
    // not found
    // FIXME: what do we do with retTypes / params here ?
  }
  
  // send return
  // TODO: notify...
  // debugging
  char hostIpAddress[ IpEndpointName::ADDRESS_STRING_LENGTH ];
  // get host ip as string
  remoteEndpoint.AddressAsString(hostIpAddress);
  std::cout << "Reply to " << hostIpAddress << ":" << remoteEndpoint.port << ".\n";
  OscSend(remoteEndpoint).send(url, res);
}


uint OscReceive::casted_type_tags(char * types, const char * pTypes)
{
  uint i = 0;
  
  types[i] = '\0';
  while(pTypes[i] && i < MAX_TYPE_TAGS_LENGTH) {
    switch (pTypes[i]) {
    case osc::DOUBLE_TYPE_TAG:
    case osc::FLOAT_TYPE_TAG:
    case osc::INT32_TYPE_TAG:
    case osc::CHAR_TYPE_TAG:
    case osc::TRUE_TYPE_TAG:
    case osc::FALSE_TYPE_TAG:
      types[i] = REAL_TYPE;
    default:
      types[i] = pTypes[i];
    }
    i++;
  }
  types[i+1] = '\0';
  return hashId(types);
}

void OscReceive::values_from_osc(Value * pRes, const osc::ReceivedMessage& pMsg)
{
  const char * type = pMsg.TypeTags();
  uint i = 0;
  Value * res = pRes;
  osc::ReceivedMessage::const_iterator arg = pMsg.ArgumentsBegin();
  osc::ReceivedMessage::const_iterator end = pMsg.ArgumentsEnd();
  
  if (arg == end) {
    // *pRet should == NULL
    return;
  }
  
  if (strlen(type) > 1) {
    res = pRes->values;
  }
  
  while (arg != end) {
    
    switch (type[i]) {
    case osc::TRUE_TYPE_TAG:
      res[i].r = 1.0;
      break;
    case osc::FALSE_TYPE_TAG:
      res[i].r = 0.0;
      break;
    // case osc::NIL_TYPE_TAG:
    //   ??
    // case osc::INFINITUM_TYPE_TAG:
    //   ??
    // zero length

    case osc::INT32_TYPE_TAG:
      res[i].r = (real_t)(arg->AsInt32Unchecked());
      break;
    case osc::FLOAT_TYPE_TAG:
      res[i].r = (real_t)(arg->AsFloatUnchecked());
      break;
    case osc::CHAR_TYPE_TAG:
      res[i].r = (real_t)(arg->AsCharUnchecked());
      break;
    case osc::DOUBLE_TYPE_TAG:
      res[i].r = (real_t)(arg->AsDoubleUnchecked());
      break;
    case osc::STRING_TYPE_TAG:
      res[i].s = arg->AsStringUnchecked();
      break;
    case osc::RGBA_COLOR_TYPE_TAG:
    case osc::MIDI_MESSAGE_TYPE_TAG:
    case osc::INT64_TYPE_TAG:
    case osc::TIME_TAG_TYPE_TAG:
    case osc::SYMBOL_TYPE_TAG:
    case osc::BLOB_TYPE_TAG:
    default:
      // ?? should not happen
      break;
    }
    i++;
    arg++;
  }
}
} // namespace oscit