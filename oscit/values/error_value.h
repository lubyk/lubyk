#ifndef _ERROR_VALUE_H_
#define _ERROR_VALUE_H_
#include "oscit/value.h"

namespace oscit {
  
/** ErrorValue is just a wrapper around an immutable const char*.*/
class ErrorValue : public Value
{ 
public:
  explicit ErrorValue() {
    set_error("");
  }
  
  explicit ErrorValue(const char *string) {
    set_error(string);
  }
};

} // oscit
#endif // _ERROR_VALUE_H_