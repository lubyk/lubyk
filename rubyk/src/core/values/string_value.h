#ifndef _NUMBER_H_
#define _NUMBER_H_
#include "value.h"
#include <string>

class String;

/* Holds the actual data of the StringValue class. This is a wrapper around an std::string. */
class StringData : public Data, public std::string
{
public:
  DATA_METHODS(StringData, StringValue)
  
  StringData(const std::string& s) : std::string(s) {}
  
  StringData(const char * s) : std::string(s) {}
  
  // copy constructor
  StringData(const StringData& v) : std::string(v) {}
  
  virtual ~StringData() {}
  
private:
  friend class String;
};

#define GET_DATA(x) const StringData * x ## _data = x.data(); \
  if (!x ## _data) return false;

#define GET_THIS_DATA() const StringData * this_data = data(); \
    if (!this_data) return false;
  
/** Value class to hold a single number (real_t). */
class String : public Value
{
public:
  VALUE_METHODS(String, StringData, StringValue)
  
  String(const std::string& s) : Value(new StringData(s)) {}
  
  String(const char * s)       : Value(new StringData(s)) {}
  
  /** Set String from std::string. */
  const std::string& operator= (const std::string& s)
  {
    *mutable_data() = s;
    return s;
  }
  
  /** Set String from const char *. */
  const char * operator= (const char * s)
  {
    *mutable_data() = s;
    return s;
  }
  
  void append(const std::string& s)
  {
    mutable_data()->append(s);
  }
  
  void append(const char * s)
  {
    mutable_data()->append(s);
  }
  
  bool operator==(const char * s)
  {
    GET_THIS_DATA()
    return *this_data == s;    
  }
  
  std::string * string() // FIXME: maybe we should return a reference...
  {
    return mPtr ? (std::string*)data_pointer() : NULL; // FIXME: shouldn't we use mutable_data() here ?
  }
  
  const std::string * string() const
  {
    return mPtr ? (std::string*)data_pointer() : NULL;
  }
};

#undef GET_THIS_DATA
#undef GET_DATA
#endif // _STRING_VALUE_H_