#ifndef _VALUE_H_
#define _VALUE_H_
#include <string>
#include "oscit/values/error.h"

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
  
  explicit Value(const char * string) : type_(STRING_VALUE) {
    set_string(string);
  }
  
  explicit Value(const std::string& string) : type_(STRING_VALUE) {
    set_string(string.c_str());
  }
  
  explicit Value(const List *list) : type_(LIST_VALUE) {
    set_list(list);
  }
  
  explicit Value(const List &list) : type_(LIST_VALUE) {
    set_list(&list);
  }
  
  explicit Value(ErrorCode code, const char *str) : type_(ERROR_VALUE) {
    set_error(code, str);
  }
  
  explicit Value(ErrorCode code, const std::string& str) : type_(ERROR_VALUE) {
    set_error(code, str.c_str());
  }
  
  /** Create a value from a TypeTag string. */
  explicit Value(TypeTag type_tag) : type_(NIL_VALUE) {
    set_type_tag(type_tag.str_);
  }
  
  /** Create a default value from a type character like 'f'. */
  explicit Value(char type_char) : type_(NIL_VALUE) {
    set_type(type_from_char(type_char));
  }
  
  /** Copy constructor (needed since many methods return a Value). */
  Value(const Value& value) : type_(NIL_VALUE) {
    *this = value;
  }
  
  /** Copy the content of the other value. */
  void operator=(const Value& other) {
    switch (other.type_) {
      case REAL_VALUE:
        set(other.r);
        break;
      case STRING_VALUE:
        set(other.s);
        break;
      case LIST_VALUE:
        set(other.list_);
        break;
      case ERROR_VALUE:
        set(other.error_);
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
  
  ValueType type() const {
    return type_;
  }
  
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
  
  /** Change the Value into an ErrorValue. */
  void set(ErrorCode code, const char *string) {
    set_type_without_default(ERROR_VALUE);
    set_error(code, string);
  }
  
  /** Change the Value into a List by copying the content of the argument. */
  void set(const Error *error) {
    set_type_without_default(ERROR_VALUE);
    set_error(error);
  }
  
  /** Change the Value into a List by copying the content of the argument. */
  void set(const Error &error) {
    set_type_without_default(ERROR_VALUE);
    set_error(&error);
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
  
  inline void set_value_at(size_t pos, const Value &val);
  
  inline size_t size() const;
  
  template<class T>
  Value &push_back(const T& elem) {
    return push_back(Value(elem));
  }
  
  inline Value &push_back(const Value& val);
  
  template<class T>
  Value &push_front(const T& elem) {
    return push_front(Value(elem));
  }
  
  inline Value &push_front(const Value& val);
  
  const std::string& error_message() {
    return error_->message();
  }
  
  ErrorCode error_code() {
    return error_->code();
  }
  
  static ValueType type_from_char(char c) {
    switch (c) {
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
  
  /** Set List content by making a copy. */
  inline void set_list(const List * list);
  
  /** Set error content. */
  void set_error(ErrorCode code, const char *string) {
    error_ = new Error(code, string);
  }
  
  /** Set error content. */
  void set_error(const Error *error) {
    error_ = new Error(*error);
  }
  
  ValueType type_;
  
public:
  union {
    Real r;
    Real f; // alias for r
    Real d; // alias for r
    char * s; // string
    List  * list_; // multi-value
    Error * error_; // error definition
  };
};

extern Value gNilValue;

} // oscit

#include "oscit/values/list.h"

namespace oscit {
const char * Value::type_tag() const {
  switch (type_) {
    case REAL_VALUE:   return "f";
    case ERROR_VALUE: // continue
    case STRING_VALUE: return "s";
    case NIL_VALUE:    return "N";
    case LIST_VALUE:   return list_->type_tag();
    default:           return "N";
  }
}

TypeTagID Value::type_tag_id() const {
  switch (type_) {
    case REAL_VALUE:   return H("f");
    case ERROR_VALUE:  // continue
    case STRING_VALUE: return H("s");
    case NIL_VALUE:    return H("N");
    case LIST_VALUE:   return list_->type_tag_id();
    default:           return H("N");
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

void Value::set_value_at(size_t pos, const Value &val) {
  if (!is_list()) return;
  list_->set_value_at(pos, val);
}

size_t Value::size() const { 
  return type_ == LIST_VALUE ? list_->size() : 0;
}

Value &Value::push_back(const Value& val) {
  if (!is_list()) {
    Value tmp(*this);
    set_type(LIST_VALUE);
    if (!tmp.is_nil()) push_back(tmp);
  }
  
  list_->push_back(Value(val));
  return *this;
}

Value &Value::push_front(const Value& val) {
  if (!is_list()) {
    Value tmp(*this);
    set_type(LIST_VALUE);
    if (!tmp.is_nil()) push_back(tmp);
  }
  
  list_->push_front(Value(val));
  return *this;
}

void Value::clear() {
  switch (type_) {
    case LIST_VALUE:
      if (list_ != NULL) delete list_;
      list_ = NULL;
      break;
    case STRING_VALUE:
      if (s != NULL) free(s);
      s = NULL;
      break;
    case ERROR_VALUE:
      if (error_ != NULL) delete error_;
      error_ = NULL;
      break;
    default:
      ; // nothing to clear
  }
}

void Value::set_default() {
  switch (type_) {
    case REAL_VALUE:
      r = 0.0;
      break;
    case STRING_VALUE:
      set_string("");
      break;
    case LIST_VALUE:
      list_ = new List;
      break;
    case ERROR_VALUE:
      error_ = new Error;
      break;
    default:
      ; // nothing to set 
  }
  
}

void Value::set_list(const List *list) {
  list_ = new List(*list);
}

std::ostream &operator<< (std::ostream &out_stream, const Value &val);

} // oscit
#endif // _VALUE_H_