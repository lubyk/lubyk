#ifndef _LOG_OBJECT_H_
#define _LOG_OBJECT_H_
#include "oscit/object.h"
#include <sstream>

class LogObject : public oscit::Object
{
public:
  LogObject(const char * name) : oscit::Object(name, ANY_TYPE_TAG_ID), logger_(std::ostringstream::out) {}
  
  virtual const Value trigger (const Value &val)
  {
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

#endif // _LOG_OBJECT_H_