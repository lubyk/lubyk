#ifndef _BANG_VALUE_H_
#define _BANG_VALUE_H_
#include "value.h"

class Bang;

/* Holds the actual data of the Bang class. This is a wrapper around a Real. */
class BangData : public Data
{
public:
  DATA_METHODS(BangData, BangValue)

  BangData(const std::string &s) {}
  
  virtual ~BangData() {}
};

/** Value class to hold a single number (Real). */
class Bang : public Value
{
public:
  VALUE_METHODS(Bang, BangData, BangValue, Value)
  VALUE_FROM_STRING(Bang)
  
  // this is the only way to instanciate a bang
  Bang(bool whatever) : Value(new BangData()) {} // TODO: replace by gBangValue.data_pointer() ?
};

#endif // _BANG_VALUE_H_