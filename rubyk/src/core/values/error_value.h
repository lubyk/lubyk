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
};

/** Value class to hold an error message (sub-class of String). */
class Error : public String
{
public:
  VALUE_METHODS(Error, ErrorData, ErrorValue)
  
  Error(const std::string& s) : String(new ErrorData(s)) {}

  Error(const char * s)       : String(new ErrorData(s)) {}
};

#endif // _ERROR_VALUE_H_