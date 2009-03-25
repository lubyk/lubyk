#ifndef _NUMBER_VALUE_H_
#define _NUMBER_VALUE_H_
#include "value.h"
#include <iostream>

class Number;

/* Holds the actual data of the Number class. This is a wrapper around a Real. */
class NumberData : public Data
{
public:
  DATA_METHODS(NumberData, NumberValue)

  NumberData(const Real& d) : mReal(d) {}
  
  // copy constructor
  NumberData(const NumberData& v)
  {
    mReal = v.mReal;
  }
  
  virtual ~NumberData() {}
  
  virtual bool set (Real& pResult)
  { 
    pResult = mReal;
    return true;
  }
  
  /** Explicit conversion to a 'Real'. */
  virtual Real convert(const Real& pDefault) const
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
  Real mReal;
};

/** Value class to hold a single number (Real). */
class Number : public Value
{
public:
  VALUE_METHODS(Number, NumberData, NumberValue, Value)
  VALUE_FROM_STRING(Number)
  
  Number(Real d) : Value(new NumberData(d)) {}
  
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
  
  /** Set Number from Real. */
  Real operator= (Real d)
  {
    mutable_data()->mReal = d;
    return d;
  }
  
  /** Add two Numbers together. */
  Real operator+(const Number& n) const
  {
    return value() + n.value();
  }
  
  /** Add a Number and a Real. */
  Real operator+(Real d) const
  {
    return value() + d;
  }
  
  /** Divide a Number by a Real. */
  Real operator/(Real d) const
  {
    return value() / d;
  }
  
  /** Add one to number. Prefix ++n. */
  Real operator++()
  {
    mutable_data()->mReal++;
    return value();
  }
                       
  /** Add one to number. Postfix n++. */
  Real operator++(int)
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
  Number& operator+=(Real d)
  {  
    mutable_data()->mReal += d;
    return *this;
  }
  
  /** Compare equality of Number with Real. */
  bool operator== (const Number& n) const
  {
    return value() == n.value();
  }
  
  /** Compare inequality of Number with Real. */
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
  
  /** Compare equality of Number with Real. */
  bool operator== (Real d) const
  {
    return value() == d;
  }
  
  /** Compare inequality of Number with Real. */
  bool operator!= (Real d) const
  {
    return value() != d;
  }
  
  /** Greater then operator. */
  bool operator>(Real d) const
  {
    return value() > d;
  }
  
  /** Smaller then operator. */
  bool operator<(Real d) const
  {
    return value() < d;
  }
  
  /** Greater or equal operator. */
  bool operator>=(Real d) const
  {
    return value() >= d;
  }
  
  /** Smaller or equal operator. */
  bool operator<=(Real d) const
  {
    return value() <= d;
  }
  
  /** Compare equality of Number with Real. */
  inline bool operator== (int d) const
  {
    return value() == ((Real)d);
  }
  
  /** Compare inequality of Number with Real. */
  inline bool operator!= (int d) const
  {
    return value() != ((Real)d);
  }
  
  /** Greater then operator. */
  bool operator>(int d) const
  {
    return value() > ((Real)d);
  }
  
  /** Smaller then operator. */
  bool operator<(int d) const
  {
    return value() < ((Real)d);
  }
  
  /** Greater or equal operator. */
  bool operator>=(int d) const
  {
    return value() >= ((Real)d);
  }
  
  /** Smaller or equal operator. */
  bool operator<=(int d) const
  {
    return value() <= ((Real)d);
  }
  
  /** Return the value. */
  Real value() const
  {
    if (ptr_) {
      return data_pointer()->mReal;
    } else {
      return 0;
    }
  }
  
  /** Type conversion to Real. */
  operator Real()
  { return value(); }
  
  /** Type conversion to Real. */
  operator int()
  { return (int)value(); }
  
private:
  friend Real operator/ (Real d, const Number& n);
  
  
  friend bool operator== (Real d, const Number& n);
  friend bool operator!= (Real d, const Number& n);
  friend bool operator> (Real d, const Number& n);
  friend bool operator>= (Real d, const Number& n);
  friend bool operator<= (Real d, const Number& n);
  friend bool operator< (Real d, const Number& n);
  friend bool operator== (int d, const Number& n);
  friend bool operator!= (int d, const Number& n);
  friend bool operator< (int d, const Number& n);
  friend bool operator> (int d, const Number& n);
  friend bool operator>= (int d, const Number& n);
  friend bool operator<= (int d, const Number& n);
};


inline Real operator/ (Real d, const Number& n)
{
  return d / n.value();
}

inline Real operator/ (int d, const Number& n)
{
  return ((Real)d) / n.value();
}


inline bool operator== (Real d, const Number& n)
{
  return d == n.value();
}
inline bool operator!= (Real d, const Number& n)
{
  return d != n.value();
}
inline bool operator> (Real d, const Number& n)
{
  return d > n.value();
}
inline bool operator>= (Real d, const Number& n)
{
  return d >= n.value();
}
inline bool operator<= (Real d, const Number& n)
{
  return d <= n.value();
}
inline bool operator< (Real d, const Number& n)
{
  return d < n.value();
}


inline bool operator== (int d, const Number& n)
{
  return ((Real)d) == n.value();
}
inline bool operator!= (int d, const Number& n)
{
  return ((Real)d) != n.value();
}
inline bool operator> (int d, const Number& n)
{
  return ((Real)d) > n.value();
}
inline bool operator>= (int d, const Number& n)
{
  return ((Real)d) >= n.value();
}
inline bool operator<= (int d, const Number& n)
{
  return ((Real)d) <= n.value();
}
inline bool operator< (int d, const Number& n)
{
  return ((Real)d) < n.value();
}

#endif // _NUMBER_VALUE_H_