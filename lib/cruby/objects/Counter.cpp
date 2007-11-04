#include "class.h"
/** This is a very basic object to see what an object definition could look like. */

class Counter : public Node
{
public:
  // params = hash, 'port:450, baud:3200, xyz:450, title:"home of the braave"'
  bool init (const Params& p)
  {
    mCounter = p.get("counter", 0);
    mIncrement     = p.get("add", 1);
    
    make_inlet<Counter,&Counter::set_counter>();
    make_inlet<Counter,&Counter::set_increment>();
    make_outlet<Counter,&Counter::value>();
    
    return true;
  }

  virtual void spy() 
  { spy_print("%i", mCounter );  }
  
  virtual void help()
  { *mOutput << "Increments by 'value' each time it receives a bang.\n"; }
  
  void set_increment(const Params& p)
  {
    mIncrement = p.get("set_increment", mIncrement);
    *mOutput << mIncrement << std::endl;
  }
  
  void get_increment()
  {
    *mOutput << mIncrement << std::endl;
  }
  
  // inlet 1
  void set_counter(const Signal& sig)
  { SET_INTEGER(mCounter); }
  
  // inlet 2
  void set_increment(const Signal& sig)
  { SET_INTEGER(mIncrement);     }
  
  // outlet 1
  void value(Signal& sig)
  { SEND_INTEGER(mCounter += mIncrement); }
  
private:
  int mCounter;
  int mIncrement;
};

extern "C" void init()
{
  Class * klass = Class::declare<Counter>("Counter");
  
  klass->add_method<Counter,&Counter::get_increment>("increment");
  klass->add_method<Counter,&Counter::set_increment>("set_increment");
}