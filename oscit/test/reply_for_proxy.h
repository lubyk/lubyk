#ifndef OSCIT_INCLUDE_OSCIT_REPLY_FORWARDER_H_
#define OSCIT_INCLUDE_OSCIT_REPLY_FORWARDER_H_

#include "oscit/object.h"
#include "oscit/root_proxy.h"

class ReplyForwarder : public Object
{
public:
  ReplyForwarder(const char * name) : Object(name, AnyIO("Forward replies to root proxies.")) {}

  virtual const Value trigger(const Value &val, const Location *origin) {
    if (!origin) return gNilValue;
    RootProxy *proxy;
    // 1. find command from protocol
    // 2. find proxy from command + location
    // 3. send 'handle_reply' to proxy

    return gNilValue;
  }
};

#endif // OSCIT_INCLUDE_OSCIT_REPLY_FOR_PROXY_H_