#ifndef OSCIT_INCLUDE_OSCIT_LOG_OBJECT_H_
#define OSCIT_INCLUDE_OSCIT_LOG_OBJECT_H_
#include "oscit/object.h"
#include <sstream>

namespace oscit {

class LogObject : public Object
{
public:
  LogObject(const char * name) : Object(name, AnyIO("Log any information.")), logger_(std::ostringstream::out) {}

  virtual const Value trigger(const Value &val, const Location *origin) {
    logger_ << val << std::endl;
    return gNilValue;
  }

  void clear() {
    logger_.str("");
  }

  const std::string str() {
    return logger_.str();
  }

private:
  std::ostringstream logger_;
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_LOG_OBJECT_H_