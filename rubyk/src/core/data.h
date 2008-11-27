#ifndef _DATA_H_
#define _DATA_H_
#include "value.h"

/** Base class for all data transmitted along objects or used as parameter/return for methods. */
class Data
{
public:
  Data() 
  {
#ifdef _TESTING_
  mId = ++sIdCounter;
#endif
  }
  
  virtual ~Data() {}
  
  virtual Data * clone()
  {
    return new Data(*this);
  }
  
  virtual value_t type() const
  { return BangValue; }
  
  virtual bool set (real_t& pResult)
  { return false; }
  
#ifdef _TESTING_
 size_t id() const
 { return mId; }

 static size_t sIdCounter;
 size_t        mId;
#endif
};

/** Macro to ease Data specialization. */
#define DATA_METHODS(klass,signature) \
  klass() {} \
  virtual Data * clone() \
  { return new klass(*this); } \
  virtual value_t type() const \
  { return signature; } \


#endif // _DATA_H_