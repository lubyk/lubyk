#ifndef _OSCIT_REAL_VALUE_H_
#define _OSCIT_REAL_VALUE_H_
#include "oscit/value.h"

namespace oscit {
  
/** RealValue is just a wrapper around a Real.*/
class RealValue : public Value
{ 
public:
  RealValue() : Value(0.0) {}
  
  RealValue(Real real) : Value(real) {}
};

} // oscit
#endif // _OSCIT_REAL_VALUE_H_