#ifndef _DUMMY_OBJECT_H_
#define _DUMMY_OBJECT_H_
#include "oscit/object.h"

#define DUMMY_OBJECT_INFO "The dummy object simply stores a value."

class DummyObject : public Object
{
public:
  /** Class signature. */
  TYPED("Object.DummyObject")

  DummyObject(const char *name, Real value) : Object(name, RangeIO(0, 127, "lux", DUMMY_OBJECT_INFO)), value_(value) {}
  DummyObject(const char *name, Value value, const Value &type) : Object(name, type), value_(value) {}
  DummyObject(const char *name, Real value, const Value &type) : Object(name, type), value_(value) {}
  DummyObject(const char *name, const char *value, const Value &type) : Object(name, type), value_(value) {}
  DummyObject(const char *name, const std::string &value, const Value &type) : Object(name, type), value_(value) {}

  virtual ~DummyObject() {}

  virtual const Value trigger(const Value &val, const Location *origin) {
    Value my_type(type()[0]);

    if (my_type.type() == val.type()) {
      value_ = val;
    }
    return value_;

    // WTF is this mess ?
    //if (my_type.is_real()) {
    //  return value_;
    //} else if (my_type.is_string()) {
    //  // testing my_type
    //  return Value("hello");
    //} else if (my_type.is_list()) {
    //  // testing multiple type
    //  return Value(name_).push_back(value_.r);
    //} else if (my_type.is_hash()) {
    //  // testing hash type
    //  return JsonValue("lazy:'dog' silly:'cats and mices'");
    //} else if (my_type.is_matrix()) {
    //  // testing matrix type
    //  Value mat(my_type);
    //  Real * data = mat.mat_data();
    //  for(size_t i=0; i < mat.mat_size(); i++) {
    //    data[i] = i;
    //  }
    //  return mat;
    //} else {
    //  return gNilValue;
    //}
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
  Value value_;
};

class DummyObject2 : public Object
{
public:
  TYPED("Object.DummyObject2")

  DummyObject2(const char * name, const char *value) : Object(name, SelectIO("rgb,rgba,yuv", "color mode", "Set color mode.")), value_(value) {}

  DummyObject2(const char * name, const char *value, const Value &type) : Object(name, type), value_(value) {}

  virtual ~DummyObject2() {}

  virtual const std::string inspect()
  {
    std::ostringstream os(std::ostringstream::out);
    os << name_ << ": " << value_.c_str();
    return os.str();
  }

  virtual const Value trigger(const Value &val, const Location *origin)
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