#ifndef _NUMBER_H_
#define _NUMBER_H_
#include "value_template.h"

class Number;

class NumberData : public Data
{
public:
  NumberData() {}
  
  NumberData(const double& d) : mValue(d) {}
  
  // copy constructor
  NumberData(const NumberData& v)
  {
    mValue = v.mValue;
  }
  
  virtual Data * clone()
  {
    return new NumberData(*this);
  }
  
  /** This method needs to be specialized for each template instanciation. */
  virtual value_t type() const
  { return NumberValue; }
  
private:
  friend class Number;
  double mValue;
};

class Number : public ValueTemplate<NumberValue, NumberData>
{
public:
  // FIXME: can we avoid these 4 constructors ?
  Number(double d) : ValueTemplate<NumberValue, NumberData>(new NumberData(d)) {}
  
  Number(NumberData* p) : ValueTemplate<NumberValue, NumberData>(p) {}
  
  Number(const Value& pOther)
  { pOther.set(this); }
  
  Number() {}
  
  double operator= (double d)
  {
    if (!mPtr) {
      mPtr = new Ptr(new NumberData(d));
    } else {
      ((NumberData*)(mPtr->mDataPtr))->mValue = d;
    }
    return d;
  }
  
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