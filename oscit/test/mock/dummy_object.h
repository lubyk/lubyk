#ifndef _DUMMY_OBJECT_H_
#define _DUMMY_OBJECT_H_
#include "oscit/base_object.h"

#define DUMMY_OBJECT_INFO "The dummy object simply stores a single real number. Call with a single 'f' argument."

class DummyObject : public oscit::BaseObject
{
public:
  DummyObject(const char * name, Real value) : oscit::BaseObject(name, H("f")), value_(value) {
    set_info(DUMMY_OBJECT_INFO);
    // current, min, max, unit
    set_type(value_.r, 0, 127, "lux"); 
  }
  
  virtual ~DummyObject() {}
  
  /** Class signature. */
  virtual uint class_type() { return H("DummyObject"); }
  
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
  
  virtual BaseObject * build_child(const std::string &name) {
    if (name == "special") {
      return adopt(new BaseObject("special"));
    }
    return NULL;
  }
  
  Real real() {
    return value_.r;
  }
  
private:
  RealValue value_;
};

class Dummy2BaseObject : public oscit::BaseObject
{
public:
  Dummy2BaseObject(const char * name, const char *value) : oscit::BaseObject(name, H("s")), value_(value) {
    set_info(DUMMY_OBJECT_INFO);
    // current, min, max, unit
    set_type(value, "rgb,rgba,yuv", "color mode"); 
  }
  
  virtual ~Dummy2BaseObject() {}
  
  virtual const std::string inspect()
  {
    std::ostringstream os(std::ostringstream::out);
    os << name_ << ": " << value_.c_str();
    return os.str();
  }
  
  virtual const Value trigger (const Value &val)
  {
    if (val.is_string())
      value_.set(val.c_str());
    return value_;
  }
  
private:
  StringValue value_;
};
#endif // _DUMMY_OBJECT_H_