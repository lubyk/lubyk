#ifndef _VALUE_H_
#define _VALUE_H_
#include <string>

namespace oscit {
  
class List;
  
enum ValueType
{
  NIL_VALUE = 0,
  REAL_VALUE,
  STRING_VALUE,
  ERROR_VALUE,
  LIST_VALUE,
};

enum ValueTypeTag
{
  NIL_TYPE_TAG    = 'N',
  REAL_TYPE_TAG   = 'f',
  STRING_TYPE_TAG = 's',
  ERROR_TYPE_TAG  = 's',
};

/** Unique identifier for osc type tags strings. */
typedef uint TypeTagID;

#define EMPTY_TAG_ID H("")

/** Value is the base type of all data transmitted between objects or used as parameters
and return values for osc messages. */
class Value
{ 
public:
  Value() : type_(NIL_VALUE) {}
  
  explicit Value(Real real) : type_(REAL_VALUE), r(real) {}
  
  explicit Value(const char * string) : type_(STRING_VALUE)
  {
    setString(string);
  }
  
  explicit Value(std::string& string) : type_(STRING_VALUE)
  {
    setString(string.c_str());
  }
  
  explicit Value(List * list) : type_(LIST_VALUE)
  {
    setList(list);
  }
  
  Value(ValueType type, const char * type_tag) : type_(NIL_VALUE)
  {
    set_type_tag(type_tag);
  }
  
  /** Create a default value from a type char. */
  explicit Value(char typeChar)
  {
    setType(typeFromChar(typeChar));
  }
  
  /** Copy constructor (needed since many methods return a Value). */
  Value(const Value& value) : type_(NIL_VALUE) {
    *this = value;
  }
  
  /** Copy the content of the other value. */
  void operator=(const Value& value) {
    switch(value.type_) {
    case REAL_VALUE:
      set(value.r);
      break;
    case STRING_VALUE:
      set(value.s);
      break;
    case LIST_VALUE:
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
  { return type_ == NIL_VALUE; }
  
  bool isReal() const
  { return type_ == REAL_VALUE; }
  
  bool isString() const
  { return type_ == STRING_VALUE; }
  
  bool isError() const
  { return type_ == ERROR_VALUE; }
  
  inline const char * type_tag() const;
  
  inline TypeTagID type_tag_id() const;
  
  /** Change the value to nil. */
  void set()
  {
    setTypeNoDefault(NIL_VALUE);
  }
  
  /** Change the Value into a RealValue. */
  void set(Real real)
  {  
    setTypeNoDefault(REAL_VALUE);
    r = real;
  }
  
  /** Change the Value into a StringValue. */
  void set(const char * string)
  {  
    setTypeNoDefault(STRING_VALUE);
    setString(string);
  }
  
  /** Change the Value into a StringValue. */
  void set(const List * pList)
  {  
    setTypeNoDefault(LIST_VALUE);
    setList(pList);
  }
  
  /** Change the Value into the specific type. Since a default value must be set,
    * it is better to use 'set'. */
  void setType(ValueType type)
  {
    setTypeNoDefault(type);
    setDefault();
  }
  
  /** Change the Value into something defined in a typeTag. */
  inline void set_type_tag(const char *type_tag);
  
  static ValueType typeFromChar(char c)
  {
    switch(c) {
      case REAL_TYPE_TAG:   return REAL_VALUE;
      case STRING_TYPE_TAG: return STRING_VALUE;
      case NIL_TYPE_TAG:    return NIL_VALUE;
      default:            return NIL_VALUE;
    }
  }
  
  inline const Value& operator[](size_t pos) const;
  
  inline Value& operator[](size_t pos);
private:
  /** Set the value to nil and release/free contained data. */
  inline void clear();
  
  /** Change the Value into the specific type. Does not set any default value
    * so the object must be considered uninitialized. */
  void setTypeNoDefault(ValueType type)
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
  
  ValueType type_;
  
public:
  union {
    Real r;
    Real f; // alias for r
    Real d; // alias for r
    char * s; // string
    List * list; // multi-value
  };
};

extern Value gNilValue;

} // oscit

#include "oscit/values/list.h"

namespace oscit {
const char * Value::type_tag() const {
  switch(type_) {
    case REAL_VALUE:   return "f";
    case STRING_VALUE: return "s";
    case NIL_VALUE:    return "N";
    case LIST_VALUE:   return list->type_tag();
    default:           return "N";
  }
}

TypeTagID Value::type_tag_id() const {
  switch(type_) {
    case REAL_VALUE:   return hashId("f");
    case STRING_VALUE: return hashId("s");
    case NIL_VALUE:    return hashId("N");
    case LIST_VALUE:   return list->type_tag_id();
    default:           return hashId("N");
  }
}

void Value::set_type_tag(const char *type_tag) {
  if (strlen(type_tag) > 1) {
    setTypeNoDefault(LIST_VALUE);
    list = new List(type_tag);
  } else {
    setType(typeFromChar(type_tag[0]));
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
  case LIST_VALUE:
    if (list != NULL) delete list;
    list = NULL;
    break;
  case STRING_VALUE:
    if (s != NULL) free(s);
    s = NULL;
    break;
  default:
    ; // nothing to clear
  }
}

void Value::setDefault() {
  switch(type_) {
  case REAL_VALUE:
    r = 0.0;
    break;
  case STRING_VALUE:
    setString("");
    break;
  case LIST_VALUE:
    list = new List;
    break;
  default:
    ; // nothing to set 
  }
  
}

void Value::setList(const List * pList) {
  list = new List(*pList);
}
} // oscit
#endif // _VALUE_H_