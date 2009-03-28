#ifndef _DUMMY_OBJECT_H_
#define _DUMMY_OBJECT_H_
#include "oscit/object.h"

class DummyObject : public oscit::Object
{
public:
  DummyObject(const char * name, Real value) : oscit::Object(name, hashId("f")), value_(value) {}
  
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