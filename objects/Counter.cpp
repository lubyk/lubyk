#include "class.h"
/** This is a very basic object to see what an object definition could look like. */

class Counter : public Node
{
public:
  // params = hash, 'port:450, baud:3200, xyz:450, title:"home of the braave"'
  bool init (const Params& p)
  {
    mCounter   = p.val("counter", 0, true);
    mIncrement = p.val("add", 1);
    
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
  { bprint(mSpy, mSpySize,"%i", mCounter );  }
  
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