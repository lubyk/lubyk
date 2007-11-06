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
    { printf("GO(%i)\n", mIncrement); sig.set(mCounter += mIncrement); }
  
private:
  int mCounter;
  int mIncrement;
};

extern "C" void init()
{
  Class * klass = Class::declare<Counter>("Counter");
  
  klass->add_method<Counter,&Counter::increment>(   "increment"     );
  
  klass->add_inlet<Counter,&Counter::set_counter>(  "set_counter"   );
  klass->add_inlet<Counter,&Counter::set_increment>("set_increment" );
  klass->add_outlet<Counter,&Counter::value>(       "value"         );
}