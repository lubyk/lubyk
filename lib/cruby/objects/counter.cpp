#include "node.h"
/** This is a very basic object to see what an object definition could look like. */

class Counter : public Node
{
public:
  // params = hash, 'port:450, baud:3200, xyz:450, title:"home of the braave"'
  bool init (const Params& p)
  {
    mCounter = p.get("counter", 0);
    
    make_inlet<Counter,&Counter::set_counter>();
    make_outlet<Counter,&Counter::increment_counter>();
    // method<Counter,&Counter::increment_counter>("increment");
    
    return true;
  }

  virtual void spy() 
  { spy_print("%i", mCounter );  }
  
  void set_counter(const Signal& sig)
  { SET_INTEGER(mCounter, sig); }
  

  void increment_counter(Signal& sig)
  { SEND_INTEGER(sig, ++mCounter); }
  
private:
  int mCounter;
};

extern "C" void init()
{
  Node::declare<Counter>("Counter");
}