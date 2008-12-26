#include "rubyk.h"
/** This is a very basic object to see what an object definition could look like. */

class Counter : public Node
{
public:
  bool init ()
  {
    mCounter = 0;
    mIncrement = 1;
    return true;
  }
  
  ATTR_ACCESSOR(mCounter, counter)
  ATTR_ACCESSOR(mIncrement, increment)
  
  // [1] set / increment counter
  void bang(const Value& val)
  { 
    if (val.is_bang()) {
      mCounter += mIncrement;
    } else {
      mCounter = val;
    }  
    send(mCounter);
  }
  
  // [2] set increment
  void increment(const Value& val)
  { 
    mIncrement = val;
  }
  
  
  virtual const Value inspect(const Value& val)  
  { 
    std::ostringstream oss; // <Counter:/v1 3 (+1)>
    oss << "<Counter:" << url() << " " << mCounter << " (";
    if (mIncrement > 0.0) oss << "+";
    oss << mIncrement << ")" << ">";
    return String(oss.str());
  }
  
private:
  Number mCounter;
  Number mIncrement;
};

extern "C" void init(Planet& planet)
{
  CLASS (Counter, "Increment counter on each bang.")
  INLET (Counter, bang, BangValue | NumberValue, "Increment counter | set counter.")
  INLET (Counter, increment, NumberValue, "Value to increment counter on each bang.")
  OUTLET(Counter, value, NumberValue, "Counter value.")
  ACCESSOR(Counter, counter, "Counter value.")
  ACCESSOR(Counter, increment, "Increment value.")
}