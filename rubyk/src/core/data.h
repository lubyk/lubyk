#ifndef _DATA_H_
#define _DATA_H_
#include "value.h"

/** Base class for all data transmitted along objects or used as parameter/return for methods. */
class Data
{
public:
  Data() {}
  
  virtual ~Data() {}
  
  virtual Data * clone()
  {
    return new Data(*this);
  }
  
  virtual value_t type() const
  { return BangValue; }
  
  virtual bool set (double * pResult)
  { return false; }
  
};

/** Macro to ease Data specialization. */
#define DATA_METHODS(klass,signature) \
  klass() {} \
  virtual Data * clone() \
  { return new NumberData(*this); } \
  virtual value_t type() const \
  { return signature; } \


#endif // _DATA_H_