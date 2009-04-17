#ifndef _DUMMY_OBJECT_H_
#define _DUMMY_OBJECT_H_
#include "oscit/object.h"

#define DUMMY_OBJECT_INFO "The dummy object simply stores a single real number. Call with a single 'f' argument."

class DummyObject : public Object
{
public:  
  /** Class signature. */
  TYPED("Object.DummyObject")
  
  DummyObject(const char * name, Real value) : Object(name, H("f")), value_(value) {
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
  
  virtual Object * build_child(const std::string &name, Value *error) {
    if (name == "special") {
      return adopt(new Object("special"));
    } else if (name == "AgeOf") {
      Object * comp = adopt(new Object(name, 0.0));
      comp->adopt(new DummyObject("Capitain", 78.0));
      return comp;
    } else if (name == "error") {
      error->set(INTERNAL_SERVER_ERROR, "You should not try to build errors !");
    }
    return NULL;
  }
  
  Real real() {
    return value_.r;
  }
  
private:
  RealValue value_;
};

class DummyObject2 : public Object
{
public:  
  TYPED("Object.DummyObject2")
  
  DummyObject2(const char * name, const char *value) : Object(name, H("s")), value_(value) {
    set_info(DUMMY_OBJECT_INFO);
    // current, min, max, unit
    set_type(value, "rgb,rgba,yuv", "color mode"); 
  }
  
  virtual ~DummyObject2() {}
  
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

class SubDummyObject : public DummyObject
{
public:
  TYPED("Object.DummyObject.SubDummyObject")
  
  SubDummyObject(const char *name, Real value) : DummyObject(name, value) {}
};
#endif // _DUMMY_OBJECT_H_