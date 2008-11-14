#ifndef _DATA_H_
#define _DATA_H_
#include "value.h"

/** Sub-classes of Data should implement:

// FIXME: can we template this ?
virtual Data * clone()
{
  return new SuperData(*this);
}

virtual value_t type() const
{ return BangValue; }

*/
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

#endif // _DATA_H_