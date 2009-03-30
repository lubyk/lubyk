#ifndef _HASH_VALUE_H_
#define _HASH_VALUE_H_
#include "oscit/value.h"

namespace oscit {
  
/** HashValue is just a wrapper around an immutable const char*.*/
class HashValue : public Value
{ 
 public:
  HashValue() : Value(TypeTag("H")) {}
};

} // oscit
#endif // _HASH_VALUE_H_