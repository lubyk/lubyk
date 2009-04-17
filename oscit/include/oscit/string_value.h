#ifndef _OSCIT_STRING_VALUE_H_
#define _OSCIT_STRING_VALUE_H_
#include "oscit/value.h"

namespace oscit {
  
/** StringValue is just a wrapper around an immutable const char*.*/
class StringValue : public Value
{ 
 public:
  StringValue() : Value("") {}
  
  StringValue(const char *string) : Value(string) {}
};

} // oscit
#endif // _OSCIT_STRING_VALUE_H_