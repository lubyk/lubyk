/** Copyright 2009 Gaspard Bucher
 *
 */

#ifndef OSCIT_INCLUDE_OSCIT_REPLY_FOR_PROXY_H_
#define OSCIT_INCLUDE_OSCIT_REPLY_FOR_PROXY_H_

class ReplyForProxy : public Object
{
public:
  ShowReplies(const char * name) : Object(name, AnyIO("Log any information.")) {}

  virtual const Value trigger(const Value &val, const Location *origin) {
    std::cout << "[" << url() << "] received " << val << std::endl;
    return gNilValue;
  }
};

#endif // OSCIT_INCLUDE_OSCIT_REPLY_FOR_PROXY_H_