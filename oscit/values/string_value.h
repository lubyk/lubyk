#ifndef _NUMBER_H_
#define _NUMBER_H_
#include "value.h"
#include <string>

class String;

/* Holds the actual data of the StringValue class. This is a wrapper around an std::string. */
class StringData : public Data
{
public:
  DATA_METHODS(StringData, StringValue)
  
  StringData(const std::string& s) : mString(s) {}
  
  StringData(const char * s) : mString(s) {}
  
  // copy constructor
  StringData(const StringData& v) : mString(v.mString) {}
  
  virtual ~StringData() {}
  
  virtual void to_stream(std::ostream& pStream) const
  {
#ifdef _TESTING_
    if (sShowId)
      pStream << "[" << mId << "] " << mString;
    else
#endif
    pStream << mString;
  }
  
  /** JSON representation of data into stream. */
  virtual void to_json(std::ostream& pStream) const
  {
    pStream << "\"" << mString << "\""; // TODO: escape "
  }
protected:
  friend class String;
  std::string mString;
};

#define GET_DATA(x) const StringData * x ## _data = x.data(); \
  if (!x ## _data) return false;

#define GET_THIS_DATA() const StringData * this_data = data(); \
    if (!this_data) return false;
  
/** Value class to hold a single number (real_t). */
class String : public Value
{
public:
  VALUE_METHODS(String, StringData, StringValue, Value)
  String(const std::string& s) : Value(new StringData(s)) {}
  String(const char* s) : Value(new StringData(std::string(s))) {}
  
  const Value& operator= (const Value val)
  { 
    val.set(*this);
    return val; 
  }
  
  /** Set String from std::string. */
  const std::string& operator= (const std::string& s)
  {
    mutable_data()->mString = s;
    return s;
  }
  
  /** Set String from const char *. */
  const char * operator= (const char * s)
  {
    mutable_data()->mString = s;
    return s;
  }
  
  String& append(const std::string& s)
  {
    mutable_data()->mString.append(s);
    return *this;
  }
  
  String& append(const char * s)
  {
    mutable_data()->mString.append(s);
    return *this;
  }
  
  size_t length () const
  {
    return string().length();
  }
  
  size_t rfind ( const std::string& str, size_t pos = std::string::npos ) const
  {
    return string().rfind(str, pos);
  }
  
  std::string substr ( size_t pos = 0, size_t n = std::string::npos ) const
  {
    return string().substr(pos, n);
  }
  
  bool operator==(const char * s)
  {
    GET_THIS_DATA()
    return this_data->mString == s;    
  }
  
  bool operator==(const std::string& s)
  {
    GET_THIS_DATA()
    return this_data->mString == s;    
  }
  
  /** Return a const version of the std::string storage. */
  inline const std::string& string() const
  {
    return ptr_ ? data_pointer()->mString : sNoString;
  }
  
  const static std::string sNoString;  /**< Empty string used as return value when no data is available. */
};

#undef GET_THIS_DATA
#undef GET_DATA
#endif // _STRING_VALUE_H_