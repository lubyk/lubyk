#ifndef _OSCIT_ERROR_H_
#define _OSCIT_ERROR_H_
#include "oscit/values.h"

namespace oscit {

/** Unique identifier for osc type tags strings. 
 *  Redefinition from value.h because of inclusion order.
 */
typedef uint TypeTagID;

enum ErrorCode {  
  BAD_REQUEST_ERROR = 400,
  NOT_FOUND_ERROR   = 404,
  INTERNAL_SERVER_ERROR = 500,
  UNKNOWN_ERROR,
};

/** A Error stores an error description with an error code. */
class Error
{
 public:
  explicit Error() : code_(UNKNOWN_ERROR), message_("unknown error") {}
  explicit Error(ErrorCode code, const char *message) : code_(code), message_(message) {}
  
  /** Copy constructor (needed because Value relies on it in its own copy constructor. */
  Error(const Error& other) : code_(other.code_), message_(other.message_) {}
  
  const char * type_tag() const { return "s"; }
  
  TypeTagID type_tag_id() const { return H("s"); }
  
  ErrorCode code() const { return code_; }
  
  void set_code(ErrorCode code) { code_ = code; }
  
  const char * str() const { return message_.c_str(); }
  
  const std::string& message() const { return message_; }
  
  void set_message(const char *string) { message_ = string; }
  
  void set_message(const std::string &string) { message_ = string; }
  
 private:
  
  ErrorCode   code_;
  std::string message_;
};

} // oscit
#endif // _OSCIT_ERROR_H_