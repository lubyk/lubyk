#ifndef _DATA_H_
#define _DATA_H_
#include "value.h"
#include <sstream>

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
  
  virtual void copy(Data * pOther)
  {
    // do nothing
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
  
  /** JSON representation of data into stream. */
  virtual void to_json(std::ostream& pStream) const
  {
    pStream << type_name();
  }
  
  /** JSON representation of data. */
  std::string to_json() const
  {
    std::ostringstream os(std::ostringstream::out);
    to_json(os);
    return os.str();
  }
  
  /** String representation of data. */
  std::string to_string() const
  {
    std::ostringstream os(std::ostringstream::out);
    to_stream(os);
    return os.str();
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
      case HashValue:
      return "Hash";
      case ErrorValue:
      return "Error";
      default:
      return "???";
    }
  }
  
  /** Explicit conversion to a 'real_t'. Returns the default value if not implemented in the specialized Data. */
  virtual Real convert(const real_t& pDefault) const
  {
    printf("Data (real_t) version in [%s]\n", type_name());
    return pDefault;
  }
  
  /** Explicit conversion to a 'real_t'. Returns the default value if not implemented in the specialized Data. */
  virtual int convert(const int& pDefault) const
  {
    printf("Data (int) version.\n");
    return pDefault;
  }
  
  /** Explicit conversion to a 'std::string'. Returns the default value if not implemented in the specialized Data. */
  virtual std::string convert(const std::string &pDefault) const
  {
    return pDefault;
  }
  
#ifdef _TESTING_
 size_t id() const
 { return mId; }

 static size_t sIdCounter;
 static bool   sShowId;
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