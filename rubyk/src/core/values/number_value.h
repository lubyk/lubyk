#ifndef _NUMBER_VALUE_H_
#define _NUMBER_VALUE_H_
#include "value.h"
#include <iostream>

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
  
  /** Explicit conversion to a 'real_t'. */
  virtual real_t convert(const real_t& pDefault) const
  {
    return mReal;
  }
  
  /** Explicit conversion to a 'int'. */
  virtual int convert(const int& pDefault) const
  {
    return (int)mReal;
  }
  
  /** Display number inside stream. */
  virtual void to_stream(std::ostream& pStream) const
  { 
#ifdef _TESTING_
    char buffer[200];
    snprintf(buffer, 200, "[%lu] %.2f", mId, mReal);
#else
    char buffer[20];
    snprintf(buffer, 20, "%.2f", mReal);
#endif
    pStream << buffer;
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
  VALUE_FROM_STRING(Number)
  
  Number(real_t d) : Value(new NumberData(d)) {}
  
  /** Set Number from Value. */
  const Value& operator= (const Value& val)
  {
    // std::cout << "[" << data_id() << "/" << ref_count() << "] operator=" << val << std::endl;
    if (val.data_type() == NumberValue) {
      if (!mPtr) {
        acquire(val);
        // std::cout << "acquired: [" << data_id() << "/" << ref_count() << "]" << std::endl;
      } else {
        copy_if_shared();
        data_pointer()->mReal = ((NumberData*)(val.data_pointer()))->mReal; // copy instead of acquire.
      }
    }
    return val;
  }
  
  /** Set Number from real_t. */
  real_t operator= (real_t d)
  {
    // std::cout << "[" << data_id() << "/" << ref_count() << "] operator=" << d << std::endl;
    if (!mPtr) {
      mPtr = new Ptr(new NumberData(d));
    } else {
      copy_if_shared();
      data_pointer()->mReal = d;
    }
    return d;
  }
  
  /** Add two Numbers together. */
  real_t operator+(const Number& n) const
  {
    return value() + n.value();
  }
  
  /** Return the value. */
  real_t value() const
  {
    if (mPtr) {
      return data_pointer()->mReal;
    } else {
      return 0;
    }
  }
  
  /** Type conversion to real_t. */
  operator real_t()
  { return value(); }
  
  /** Type conversion to real_t. */
  operator int()
  { return (int)value(); }
  
};

#endif // _NUMBER_VALUE_H_