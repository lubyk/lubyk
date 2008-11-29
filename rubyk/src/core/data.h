#ifndef _DATA_H_
#define _DATA_H_
#include "value.h"
#include <ostream>

/** Base class for all data transmitted along objects or used as parameter/return for methods. */
class Data
{
public:
  Data() 
  {
#ifdef _TESTING_
  mId = ++sIdCounter;
#endif
  }
  
  virtual ~Data() {}
  
  virtual Data * clone()
  {
    return new Data(*this);
  }
  
  virtual value_t type() const
  { return BangValue; }
  
  const char * type_name() const
  { return name_from_type(type()); }
  
  virtual bool set (real_t& pResult)
  { return false; }
  
  /** String representation of data into stream. */
  virtual void to_stream(std::ostream& pStream) const
  {
    pStream << type_name();
  }
  
  /** Return the textual representation of a value from a value type. */
  static const char* name_from_type(value_t pType)
  {  
    switch (pType)
    {
      case NilValue:
      return "Nil";
      case BangValue:
      return "Bang!";
      case AnonymousValue:
      return "Anonymous";
      case NumberValue:
      return "Number";
      case MatrixValue:
      return "Matrix";
      case StringValue:
      return "String";
      case CharMatrixValue:
      return "CharMatrix";
      case CommandValue:
      return "Command";
      case DictionaryValue:
      return "Dictionary";
      case ErrorValue:
      return "Error";
      default:
      return "???";
    }
  }
  
#ifdef _TESTING_
 size_t id() const
 { return mId; }

 static size_t sIdCounter;
 size_t        mId;
#endif
};

/** Macro to ease Data specialization. */
#define DATA_METHODS(klass,signature) \
  klass() {} \
  virtual Data * clone() \
  { return new klass(*this); } \
  virtual value_t type() const \
  { return signature; } \


#endif // _DATA_H_