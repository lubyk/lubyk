#ifndef _DUMMY_OBJECT_H_
#define _DUMMY_OBJECT_H_
#include "oscit/object.h"

#define DUMMY_OBJECT_INFO "The dummy object simply stores a single real number."

class DummyObject : public Object
{
public:  
  /** Class signature. */
  TYPED("Object.DummyObject")
  
  DummyObject(const char * name, Real value) : Object(name, RangeInput(0, 127, "lux", DUMMY_OBJECT_INFO)), value_(value) {}
  DummyObject(const char *name, Real value, const Value &type) : Object(name, type), value_(value) {}
  
  virtual ~DummyObject() {}
  
  virtual const std::string inspect()
  {
    std::ostringstream os(std::ostringstream::out);
    os << name_ << ": " << value_.r;
    return os.str();
  }
  
  virtual const Value trigger (const Value &val)
  {
    if (val.is_real()) value_.r = val.r;
    
    if (type()[0].is_real()) {
      return value_;
    } else if (type()[0].is_string()) {
      // testing type
      return Value("hello");
    } else if (type()[0].is_list()) {
      // testing multiple type
      return Value(name_).push_back(value_.r);
    } else {
      return gNilValue;
    }
  }
  
  virtual Object * build_child(const std::string &name, Value *error) {
    if (name == "special") {
      return adopt(new Object("special"));
    } else if (name == "AgeOf") {
      Object * comp = adopt(new Object(name));
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
  
  DummyObject2(const char * name, const char *value) : Object(name, SelectInput("rgb,rgba,yuv", "color mode", "Set color mode.")), value_(value) {}
  
  DummyObject2(const char * name, const char *value, const Value &type) : Object(name, type), value_(value) {}
  
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