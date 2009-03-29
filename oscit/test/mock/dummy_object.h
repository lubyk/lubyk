#ifndef _DUMMY_OBJECT_H_
#define _DUMMY_OBJECT_H_
#include "oscit/object.h"

#define DUMMY_OBJECT_INFO "The dummy object simply stores a single real number. Call with a single 'f' argument."

class DummyObject : public oscit::Object
{
public:
  DummyObject(const char * name, Real value) : oscit::Object(name, H("f")), value_(value) {
    set_info(DUMMY_OBJECT_INFO);
    // current, min, max, unit
    set_type(value_.r, 0, 127, "lux"); 
  }
  
  virtual ~DummyObject() {}
  
  virtual const std::string inspect()
  {
    std::ostringstream os(std::ostringstream::out);
    os << name_ << ": " << value_.r;
    return os.str();
  }
  
  virtual const Value trigger (const Value &val)
  {
    if (val.is_real())
      value_.r = val.r;
    return value_;
  }
  
  virtual Object * build_child(const std::string &name) {
    if (name == "special") {
      return adopt(new Object("special"));
    }
    return NULL;
  }
  
private:
  RealValue value_;
};

class Dummy2Object : public oscit::Object
{
public:
  Dummy2Object(const char * name, const char *value) : oscit::Object(name, H("s")), value_(value) {
    set_info(DUMMY_OBJECT_INFO);
    // current, min, max, unit
    set_type(value, "rgb,rgba,yuv", "color mode"); 
  }
  
  virtual ~Dummy2Object() {}
  
  virtual const std::string inspect()
  {
    std::ostringstream os(std::ostringstream::out);
    os << name_ << ": " << value_.s;
    return os.str();
  }
  
  virtual const Value trigger (const Value &val)
  {
    if (val.is_string())
      value_.set(val.s);
    return value_;
  }
  
private:
  StringValue value_;
};
#endif // _DUMMY_OBJECT_H_