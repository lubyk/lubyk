#include "class.h"
/** This is a very basic object to see what an object definition could look like. */

class Counter : public Node
{
public:
  // params = hash, 'port:450, baud:3200, xyz:450, title:"home of the braave"'
  bool init (const Params& p)
  {
    mCounter   = p.val("counter", 0);
    mIncrement = p.val("add", 1);
    
    return true;
  }

  virtual void spy() 
  { spy_print("%i", mCounter );  }
  
  virtual void help()
  { *mOutput << "Increments by 'value' each time it receives a bang.\n"; }
  
  void increment()
  {
    *mOutput << mIncrement << std::endl;
  }
  
  // inlet 1
  void set_counter(const Signal& sig)
  { sig.get(&mCounter); }
  
  // inlet 2
  void set_increment(const Signal& sig)
  { sig.get(&mIncrement);     }
  
  // outlet 1
  void value(Signal& sig)
  { sig.set(mCounter += mIncrement); }
  
private:
  int mCounter;
  int mIncrement;
};

extern "C" void init()
{
  CLASS (Counter)
  INLET (Counter, set_counter)
  INLET (Counter, set_increment)
  OUTLET(Counter, value)
  METHOD(Counter, increment)
}