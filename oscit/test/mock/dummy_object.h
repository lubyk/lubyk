#ifndef _DUMMY_OBJECT_H_
#define _DUMMY_OBJECT_H_
#include "oscit/object.h"

#define DUMMY_OBJECT_INFO "The dummy object simply stores a single real number. Call with a single 'f' argument."

class DummyObject : public oscit::Object
{
public:
  DummyObject(const char * name, Real value) : oscit::Object(name, hashId("f")), value_(value) {
    set_info(DUMMY_OBJECT_INFO);
  }
  
  virtual ~DummyObject() {}
  
  virtual const std::string inspect()
  {
    std::ostringstream os(std::ostringstream::out);
    os << name_ << ": " << value_.r;
    return os.str();
  }
  
  virtual const Value trigger (const Value& val)
  {
    if (val.is_real())
      value_.r = val.r;
    return value_;
  }
private:
  RealValue value_;
};

#endif // _DUMMY_OBJECT_H_