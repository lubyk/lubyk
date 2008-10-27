#include "class.h"
/** This is a very basic object to see what an object definition could look like. */

class Counter : public Node
{
public:
  bool init (const Params& p)
  {
    mCounter = 0;
    mIncrement = 1;
    return true;
  }
  
  bool set (const Params& p)
  {
    mCounter   = p.val("counter",   mCounter, true);
    mIncrement = p.val("increment", mIncrement);
    
    return true;
  }

  // inlet 1
  void bang(const Signal& sig)
  { 
    sig.get(&mCounter);
    
    send(mCounter += mIncrement);
  }
  
  // inlet 2
  void set_increment(const Signal& sig)
  { sig.get(&mIncrement);     }
  
  
  virtual void spy() 
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