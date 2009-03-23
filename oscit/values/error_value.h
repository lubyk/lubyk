#ifndef _ERROR_VALUE_H_
#define _ERROR_VALUE_H_
#include "string_value.h"

class Error;

/* Holds the actual data of the Error class. This is a sub-class of StringData / std::string. */
class ErrorData : public StringData
{
public:
  DATA_METHODS(ErrorData, ErrorValue)
  
  ErrorData(const std::string& s) : StringData(s) {}

  ErrorData(const char * s) : StringData(s) {}
  
  // copy constructor
  ErrorData(const ErrorData& v) : StringData(v) {}
  
  virtual ~ErrorData() {}
  
  virtual void to_stream(std::ostream& pStream) const
  {
#ifdef _TESTING_
    if (sShowId)
      pStream << "[" << mId << "]" << " #" << mString;
    else
#endif
    pStream << "#" << mString;
  }
  
  /** JSON representation of data into stream. */
  virtual void to_json(std::ostream& pStream) const
  {
    pStream << "#\"" << mString << "\"";
  }
private:
  friend class Error;
};

/** Value class to hold an error message (sub-class of String). */
class Error : public String
{
public:
  VALUE_METHODS(Error, ErrorData, ErrorValue, String)
  Error(const std::string& s) : String(new ErrorData(s)) {}
  Error(const char* s) : String(new ErrorData(std::string(s))) {}
  
  /** Set Error from std::string. */
  const std::string& operator= (const std::string& s)
  {
    mutable_data()->mString = s;
    return s;
  }
  
  /** Set Error from const char *. */
  const char * operator= (const char * s)
  {
    mutable_data()->mString = s;
    return s;
  }
  
  const char * message() const
  {
    if (mPtr) {
      return data_pointer()->mString.c_str();
    } else {
      return "";
    }
  }
};

#endif // _ERROR_VALUE_H_