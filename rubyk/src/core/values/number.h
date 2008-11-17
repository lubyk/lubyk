#ifndef _NUMBER_H_
#define _NUMBER_H_
#include "value.h"

class Number;

/* Holds the actual data of the Number class. This is a wrapper around a double. */
class NumberData : public Data
{
public:
  DATA_METHODS(NumberData, NumberValue)
  
  NumberData(const double& d) : mValue(d) {}
  
  // copy constructor
  NumberData(const NumberData& v)
  {
    mValue = v.mValue;
  }
  
  virtual ~NumberData() {}
  
private:
  friend class Number;
  double mValue;
};

/** Value class to hold a single number (double). */
class Number : public Value
{
public:
  VALUE_METHODS(Number, NumberData, NumberValue)
  
  Number(double d) : Value(new NumberData(d)) {}
  
  /** Set Number from double. */
  double operator= (double d)
  {
    if (!mPtr) {
      mPtr = new Ptr(new NumberData(d));
    } else {
      copy_if_shared();
      ((NumberData*)(mPtr->mDataPtr))->mValue = d;
    }
    return d;
  }
  
  /** Return the value. */
  double value()
  {
    if (mPtr) {
      return data_pointer()->mValue;
    } else {
      return 0;
    }
  }
};

#endif // _NUMBER_H_