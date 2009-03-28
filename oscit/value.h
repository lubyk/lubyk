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

/** Wrapper around a string identifying an osc type list. */
struct TypeTag
{
  explicit TypeTag(const char * str) : str_(str) {}
  const char * str_;
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
    set_string(string);
  }
  
  explicit Value(const std::string& string) : type_(STRING_VALUE)
  {
    set_string(string.c_str());
  }
  
  explicit Value(const List *list) : type_(LIST_VALUE)
  {
    set_list(list);
  }
  
  explicit Value(const List &list) : type_(LIST_VALUE)
  {
    set_list(&list);
  }
  
  explicit Value(TypeTag type_tag) : type_(NIL_VALUE)
  {
    set_type_tag(type_tag.str_);
  }
  
  /** Create a default value from a type character like 'f'. */
  explicit Value(char type_char) : type_(NIL_VALUE)
  {
    set_type(type_from_char(type_char));
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
      set(value.list_);
      break;
    default:
      set();
    }
  }
  
  ~Value()
  {
    clear();
  }
  
  bool is_nil() const    { return type_ == NIL_VALUE; }
  
  bool is_real() const   { return type_ == REAL_VALUE; }
  
  bool is_string() const { return type_ == STRING_VALUE; }
  
  bool is_list() const   { return type_ == LIST_VALUE; }
  
  bool is_error() const  { return type_ == ERROR_VALUE; }
  
  inline const char * type_tag() const;
  
  inline TypeTagID type_tag_id() const;
  
  /** Change the value to nil. */
  void set() {
    set_type_without_default(NIL_VALUE);
  }
  
  /** Change the Value into a RealValue. */
  void set(Real real) {
    set_type_without_default(REAL_VALUE);
    r = real;
  }
  
  /** Change the Value into a StringValue. */
  void set(const char *string) {
    set_type_without_default(STRING_VALUE);
    set_string(string);
  }
  
  /** Change the Value into an ErrorValue. */
  void set_error(const char *string) {
    set_type_without_default(ERROR_VALUE);
    set_string(string);
  }
  
  /** Change the Value into a List by copying the content of the argument. */
  void set(const List *list) {
    set_type_without_default(LIST_VALUE);
    set_list(list);
  }
  
  /** Change the Value into a List by copying the content of the argument. */
  void set(const List &list) {
    set_type_without_default(LIST_VALUE);
    set_list(&list);
  }
  
  /** Change the Value into the specific type. Since a default value must be set,
    * it is better to use 'set'. */
  void set_type(ValueType type) {
    set_type_without_default(type);
    set_default();
  }
  
  /** Change the Value into something defined in a typeTag. */
  inline void set_type_tag(const char *type_tag);
  
  inline const Value& operator[](size_t pos) const;
  
  inline Value& operator[](size_t pos);
  
  inline size_t size() const;
  
  template<class T>
  void append(const T& elem) {
    append(Value(elem));
  }
  
  inline void append(const Value& val);
  
  static ValueType type_from_char(char c) {
    switch(c) {
      case REAL_TYPE_TAG:   return REAL_VALUE;
      case STRING_TYPE_TAG: return STRING_VALUE;
      case NIL_TYPE_TAG:    return NIL_VALUE;
      default:              return NIL_VALUE;
    }
  }
  
  
 private:
  /** Set the value to nil and release/free contained data. */
  inline void clear();
  
  /** Change the Value into the specific type. Does not set any default value
    * so the object must be considered uninitialized. */
  void set_type_without_default(ValueType type) {
    clear();
    type_ = type;
  }
  
  /** Properly initialize the type with a default value. */
  inline void set_default();
  
  void set_string(const char * string) {
    size_t len = strlen(string) + 1;
    s = (char*)malloc(sizeof(char) * len);
    memcpy(s,string,len);
  }
  
  /** Set value to list by making a copy. */
  inline void set_list(const List * list);
  
  ValueType type_;
  
public:
  union {
    Real r;
    Real f; // alias for r
    Real d; // alias for r
    char * s; // string
    char * e; // error string
    List * list_; // multi-value
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
    case LIST_VALUE:   return list_->type_tag();
    default:           return "N";
  }
}

TypeTagID Value::type_tag_id() const {
  switch(type_) {
    case REAL_VALUE:   return hashId("f");
    case STRING_VALUE: return hashId("s");
    case NIL_VALUE:    return hashId("N");
    case LIST_VALUE:   return list_->type_tag_id();
    default:           return hashId("N");
  }
}

void Value::set_type_tag(const char *type_tag) {
  if (strlen(type_tag) > 1) {
    set_type_without_default(LIST_VALUE);
    list_ = new List(type_tag);
  } else {
    set_type(type_from_char(type_tag[0]));
  }
}

const Value& Value::operator[](size_t pos) const {
  return (*list_)[pos];
}

Value& Value::operator[](size_t pos) {
  return (*list_)[pos];
}

size_t Value::size() const { 
  return type_ == LIST_VALUE ? list_->size() : 0;
}

void Value::append(const Value& val) {
  if (!is_list()) set_type(LIST_VALUE);
  list_->append(Value(val));
}

void Value::clear() {
  switch(type_) {
  case LIST_VALUE:
    if (list_ != NULL) delete list_;
    list_ = NULL;
    break;
  case STRING_VALUE:
    if (s != NULL) free(s);
    s = NULL;
    break;
  default:
    ; // nothing to clear
  }
}

void Value::set_default() {
  switch(type_) {
  case REAL_VALUE:
    r = 0.0;
    break;
  case STRING_VALUE:
    set_string("");
    break;
  case LIST_VALUE:
    list_ = new List;
    break;
  default:
    ; // nothing to set 
  }
  
}

void Value::set_list(const List * list) {
  list_ = new List(*list);
}
} // oscit
#endif // _VALUE_H_