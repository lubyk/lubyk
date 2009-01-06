#ifndef _TEST_HELPER_H_
#define _TEST_HELPER_H_
#include <cxxtest/TestSuite.h>
#include "oscit/object.h"
#include "oscit/root.h"
#include "oscit/globals.cpp"

class TestHelper : public CxxTest::TestSuite
{
  
};

class DoubleObject : public oscit::Object
{
public:
  DoubleObject(const char * name, real_t pVal = 0) : Object("d", name), mVal(pVal) {}
  
  virtual const Value trigger (const Value val)
  {
    mVal = val.r;
    return Value(mVal);
  }
  
  real_t mVal;
};

#endif // _TEST_HELPER_H_