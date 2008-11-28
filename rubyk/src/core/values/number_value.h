#ifndef _NUMBER_VALUE_H_
#define _NUMBER_VALUE_H_
#include "value.h"

class Number;

/* Holds the actual data of the Number class. This is a wrapper around a real_t. */
class NumberData : public Data
{
public:
  DATA_METHODS(NumberData, NumberValue)
  
  NumberData(const real_t& d) : mReal(d) {}
  
  // copy constructor
  NumberData(const NumberData& v)
  {
    mReal = v.mReal;
  }
  
  virtual ~NumberData() {}
  
  virtual bool set (real_t& pResult)
  { 
    pResult = mReal;
    return true;
  }
  
private:
  friend class Number;
  real_t mReal;
};

/** Value class to hold a single number (real_t). */
class Number : public Value
{
public:
  VALUE_METHODS(Number, NumberData, NumberValue, Value)
  
  Number(real_t d) : Value(new NumberData(d)) {}
  
  /** Set Number from real_t. */
  real_t operator= (real_t d)
  {
    if (!mPtr) {
      mPtr = new Ptr(new NumberData(d));
    } else {
      copy_if_shared();
      data_pointer()->mReal = d;
    }
    return d;
  }
  
  /** Return the value. */
  real_t value()
  {
    if (mPtr) {
      return data_pointer()->mReal;
    } else {
      return 0;
    }
  }
};

#endif // _NUMBER_VALUE_H_