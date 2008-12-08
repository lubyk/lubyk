#include "rubyk.h"
/** This is a very basic object to see what an object definition could look like. */

class Counter : public Node
{
public:
  bool init (const Value& p)
  {
    mCounter = 0;
    mIncrement = 1;
    return true;
  }
  
  bool set (const Value& p)
  {
    mCounter   = p.val("counter",   mCounter, true);
    mIncrement = p.val("increment", mIncrement);
    
    return true;
  }

  // inlet 1
  void bang(const Value& val)
  { 
    sig.get(&mCounter);
    
    send(mCounter += mIncrement);
  }
  
  // inlet 2
  void set_increment(const Value& val)
  { sig.get(&mIncrement);     }
  
  
  virtual const Value inspect(const Value& val)  
  { bprint(mSpy, mSpySize,"%i (%+i)", mCounter, mIncrement );  }
  
  virtual void help()
  { *mOutput << "Increments by 'value' each time it receives a bang.\n"; }
  
  void increment()
  {
    *mOutput << mIncrement << std::endl;
  }
  
private:
  int mCounter;
  int mIncrement;
};

extern "C" void init()
{
  CLASS (Counter)
  INLET (Counter, set_increment)
  OUTLET(Counter, value)
  METHOD(Counter, increment)
}