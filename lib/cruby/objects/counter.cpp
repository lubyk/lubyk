#include "node.h"
/** This is a very basic object to see what an object definition could look like. */

class Counter : public Node
{
public:
  // params = hash, 'port:450, baud:3200, xyz:450, title:"home of the braave"'
  bool init (const Params& p)
  {
    mCounter = p.get("counter", 0);
    mAdd     = p.get("add", 1);
    
    make_inlet<Counter,&Counter::set_add>();
    make_inlet<Counter,&Counter::set_counter>();
    make_outlet<Counter,&Counter::value>();
    // method<Counter,&Counter::increment_counter>("increment");
    
    return true;
  }

  virtual void spy() 
  { spy_print("%i", mCounter );  }
  
  
  void set_add(const Signal& sig)
  { SET_INTEGER(mAdd, sig);     }
  
  void set_counter(const Signal& sig)
  { SET_INTEGER(mCounter, sig); }
  

  void value(Signal& sig)
  { SEND_INTEGER(sig, mCounter += mAdd); }
  
private:
  int mCounter;
  int mAdd;
};

extern "C" void init()
{
  Node::declare<Counter>("Counter");
}