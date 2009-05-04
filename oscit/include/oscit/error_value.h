#ifndef OSCIT_INCLUDE_OSCIT_ERROR_VALUE_H_
#define OSCIT_INCLUDE_OSCIT_ERROR_VALUE_H_
#include "oscit/value.h"

namespace oscit {
  
/** ErrorValue is just a wrapper around an immutable const char*.*/
class ErrorValue : public Value
{ 
public:
  explicit ErrorValue() {
    set_type(ERROR_VALUE);
  }
  
  explicit ErrorValue(ErrorCode code, const char *string) : Value(code, string) {}
  
  explicit ErrorValue(ErrorCode code, const std::string &string) : Value(code, string) {}
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_ERROR_VALUE_H_
