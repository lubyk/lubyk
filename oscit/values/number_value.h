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
    if (sShowId)
      snprintf(buffer, 200, "[%lu] %.2f", mId, mReal);
    else
      snprintf(buffer, 200, "%.2f", mReal);
#else
    char buffer[20];
    snprintf(buffer, 20, "%.2f", mReal);
#endif
    pStream << buffer;
  }
  
  /** JSON representation of data into stream. */
  virtual void to_json(std::ostream& pStream) const
  {
    char buffer[20];
    snprintf(buffer, 20, "%.2f", mReal);
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
    // std::cout << "[" << data_id() << "/" << getRefCount() << "] operator=" << val << std::endl;
    if (val.data_type() == NumberValue) {
      if (!ptr_) {
        acquire(val);
        // std::cout << "acquired: [" << data_id() << "/" << getRefCount() << "]" << std::endl;
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
    mutable_data()->mReal = d;
    return d;
  }
  
  /** Add two Numbers together. */
  real_t operator+(const Number& n) const
  {
    return value() + n.value();
  }
  
  /** Add a Number and a real_t. */
  real_t operator+(real_t d) const
  {
    return value() + d;
  }
  
  /** Divide a Number by a real_t. */
  real_t operator/(real_t d) const
  {
    return value() / d;
  }
  
  /** Add one to number. Prefix ++n. */
  real_t operator++()
  {
    mutable_data()->mReal++;
    return value();
  }
                       
  /** Add one to number. Postfix n++. */
  real_t operator++(int)
  {
    mutable_data()->mReal++;
    return value() - 1;
  }
  
  /** Add two Numbers together. */
  Number& operator+=(const Number& n)
  {
    mutable_data()->mReal += n.value();
    return *this;
  }
  
  /** Add two Numbers together. */
  Number& operator+=(real_t d)
  {  
    mutable_data()->mReal += d;
    return *this;
  }
  
  /** Compare equality of Number with real_t. */
  bool operator== (const Number& n) const
  {
    return value() == n.value();
  }
  
  /** Compare inequality of Number with real_t. */
  bool operator!= (const Number& n) const
  {
    return value() != n.value();
  }
  
  /** Greater then operator. */
  bool operator>(const Number& n) const
  {
    return value() > n.value();
  }
  
  /** Smaller then operator. */
  bool operator<(const Number& n) const
  {
    return value() < n.value();
  }
  
  /** Greater or equal operator. */
  bool operator>=(const Number& n) const
  {
    return value() >= n.value();
  }
  
  /** Smaller or equal operator. */
  bool operator<=(const Number& n) const
  {
    return value() <= n.value();
  }
  
  /** Compare equality of Number with real_t. */
  bool operator== (real_t d) const
  {
    return value() == d;
  }
  
  /** Compare inequality of Number with real_t. */
  bool operator!= (real_t d) const
  {
    return value() != d;
  }
  
  /** Greater then operator. */
  bool operator>(real_t d) const
  {
    return value() > d;
  }
  
  /** Smaller then operator. */
  bool operator<(real_t d) const
  {
    return value() < d;
  }
  
  /** Greater or equal operator. */
  bool operator>=(real_t d) const
  {
    return value() >= d;
  }
  
  /** Smaller or equal operator. */
  bool operator<=(real_t d) const
  {
    return value() <= d;
  }
  
  /** Compare equality of Number with real_t. */
  inline bool operator== (int d) const
  {
    return value() == ((real_t)d);
  }
  
  /** Compare inequality of Number with real_t. */
  inline bool operator!= (int d) const
  {
    return value() != ((real_t)d);
  }
  
  /** Greater then operator. */
  bool operator>(int d) const
  {
    return value() > ((real_t)d);
  }
  
  /** Smaller then operator. */
  bool operator<(int d) const
  {
    return value() < ((real_t)d);
  }
  
  /** Greater or equal operator. */
  bool operator>=(int d) const
  {
    return value() >= ((real_t)d);
  }
  
  /** Smaller or equal operator. */
  bool operator<=(int d) const
  {
    return value() <= ((real_t)d);
  }
  
  /** Return the value. */
  real_t value() const
  {
    if (ptr_) {
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
  
private:
  friend real_t operator/ (real_t d, const Number& n);
  
  
  friend bool operator== (real_t d, const Number& n);
  friend bool operator!= (real_t d, const Number& n);
  friend bool operator> (real_t d, const Number& n);
  friend bool operator>= (real_t d, const Number& n);
  friend bool operator<= (real_t d, const Number& n);
  friend bool operator< (real_t d, const Number& n);
  friend bool operator== (int d, const Number& n);
  friend bool operator!= (int d, const Number& n);
  friend bool operator< (int d, const Number& n);
  friend bool operator> (int d, const Number& n);
  friend bool operator>= (int d, const Number& n);
  friend bool operator<= (int d, const Number& n);
};


inline real_t operator/ (real_t d, const Number& n)
{
  return d / n.value();
}

inline real_t operator/ (int d, const Number& n)
{
  return ((real_t)d) / n.value();
}


inline bool operator== (real_t d, const Number& n)
{
  return d == n.value();
}
inline bool operator!= (real_t d, const Number& n)
{
  return d != n.value();
}
inline bool operator> (real_t d, const Number& n)
{
  return d > n.value();
}
inline bool operator>= (real_t d, const Number& n)
{
  return d >= n.value();
}
inline bool operator<= (real_t d, const Number& n)
{
  return d <= n.value();
}
inline bool operator< (real_t d, const Number& n)
{
  return d < n.value();
}


inline bool operator== (int d, const Number& n)
{
  return ((real_t)d) == n.value();
}
inline bool operator!= (int d, const Number& n)
{
  return ((real_t)d) != n.value();
}
inline bool operator> (int d, const Number& n)
{
  return ((real_t)d) > n.value();
}
inline bool operator>= (int d, const Number& n)
{
  return ((real_t)d) >= n.value();
}
inline bool operator<= (int d, const Number& n)
{
  return ((real_t)d) <= n.value();
}
inline bool operator< (int d, const Number& n)
{
  return ((real_t)d) < n.value();
}

#endif // _NUMBER_VALUE_H_