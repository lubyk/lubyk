#ifndef _VALUE_H_
#define _VALUE_H_
#include "oscit/conf.h"

namespace oscit {
  
class List;
  
enum value_t
{
  NilValueType = 0,
  RealValueType,
  StringValueType,
  ErrorValueType,
  ListValueType,
};

enum value_type_tag_t
{
  NilTypeTag =  'N',
  RealTypeTag = 'f',
  StringTypeTag = 's',
  ErrorTypeTag  = 's',
};

/** Value is the base type of all data transmitted between objects or used as parameters
and return values for osc messages. */
class Value
{ 
public:
  Value() : type_(NilValueType) {}
  
  explicit Value(real_t real) : type_(RealValueType), r(real) {}
  
  explicit Value(const char * string) : type_(StringValueType)
  {
    setString(string);
  }
  
  explicit Value(std::string& string) : type_(StringValueType)
  {
    setString(string.c_str());
  }
  
  explicit Value(List * list) : type_(ListValueType)
  {
    setList(list);
  }
  
  Value(value_t type, const char * typeTag) : type_(NilValueType)
  {
    setTypeTag(typeTag);
  }
  
  /** Create a default value from a type char. */
  explicit Value(char typeChar)
  {
    setType(typeFromChar(typeChar));
  }
  
  /** Copy constructor (needed since many methods return a Value). */
  Value(const Value& value) : type_(NilValueType) {
    *this = value;
  }
  
  /** Copy the content of the other value. */
  void operator=(const Value& value) {
    switch(value.type_) {
    case RealValueType:
      set(value.r);
      break;
    case StringValueType:
      set(value.s);
      break;
    case ListValueType:
      set(value.list);
      break;
    default:
      set();
    }
  }
  
  ~Value()
  {
    clear();
  }
  
  bool isNil() const
  { return type_ == NilValueType; }
  
  bool isReal() const
  { return type_ == RealValueType; }
  
  bool isString() const
  { return type_ == StringValueType; }
  
  bool isError() const
  { return type_ == ErrorValueType; }
  
  inline const char * getTypeTag() const;
  
  /** Change the value to nil. */
  void set()
  {
    setTypeNoDefault(NilValueType);
  }
  
  /** Change the Value into a RealValue. */
  void set(real_t real)
  {  
    setTypeNoDefault(RealValueType);
    r = real;
  }
  
  /** Change the Value into a StringValue. */
  void set(const char * string)
  {  
    setTypeNoDefault(StringValueType);
    setString(string);
  }
  
  /** Change the Value into a StringValue. */
  void set(const List * pList)
  {  
    setTypeNoDefault(ListValueType);
    setList(pList);
  }
  
  /** Change the Value into the specific type. Since a default value must be set,
    * it is better to use 'set'. */
  void setType(value_t type)
  {
    setTypeNoDefault(type);
    setDefault();
  }
  
  /** Change the Value into something defined in a typeTag. */
  inline void setTypeTag(const char * typeTag);
  
  static value_t typeFromChar(char c)
  {
    switch(c) {
      case RealTypeTag:   return RealValueType;
      case StringTypeTag: return StringValueType;
      case NilTypeTag:    return NilValueType;
      default:            return NilValueType;
    }
  }
  
  inline const Value& operator[](size_t pos) const;
  
  inline Value& operator[](size_t pos);
private:
  /** Set the value to nil and release/free contained data. */
  inline void clear();
  
  /** Change the Value into the specific type. Does not set any default value
    * so the object must be considered uninitialized. */
  void setTypeNoDefault(value_t type)
  {
    clear();
    type_ = type;
  }
  
  /** Properly initialize the type with a default value. */
  inline void setDefault();
  
  void setString(const char * string)
  {
    size_t len = strlen(string) + 1;
    s = (char*)malloc(sizeof(char) * len);
    memcpy(s,string,len);
  }
  
  inline void setList(const List * pList);
  
  uint type_;
  
public:
  union {
    real_t r;
    real_t f; // alias for r
    real_t d; // alias for r
    char * s; // string
    List * list; // multi-value
  };
};

extern Value gNilValue;

} // oscit

#include "oscit/values/list.h"

namespace oscit {
const char * Value::getTypeTag() const {
  switch(type_) {
  case RealValueType:   return "f";
  case StringValueType: return "s";
  case NilValueType:    return "N";
  case ListValueType:   return list->getTypeTag();
  default:              return "N";
  }
}

void Value::setTypeTag(const char * typeTag) {
  if (strlen(typeTag) > 1) {
    setTypeNoDefault(ListValueType);
    list = new List(typeTag);
  } else {
    setType(typeFromChar(typeTag[0]));
  }
}

const Value& Value::operator[](size_t pos) const {
  return (*list)[pos];
}

Value& Value::operator[](size_t pos) {
  return (*list)[pos];
}

void Value::clear() {
  switch(type_) {
  case ListValueType:
    if (list != NULL) delete list;
    list = NULL;
    break;
  case StringValueType:
    if (s != NULL) free(s);
    s = NULL;
    break;
  }
}

void Value::setDefault() {
  switch(type_) {
  case RealValueType:
    r = 0.0;
    break;
  case StringValueType:
    setString("");
    break;
  case ListValueType:
    list = new List;
    break;
  }
  
}

void Value::setList(const List * pList) {
  list = new List(*pList);
}
} // oscit
#endif // _VALUE_H_