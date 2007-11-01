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

  virtual const char * spy() 
  { 
    sprintf(mSpy, "%i", mCounter);
    return mSpy;
  }
  
  void set_counter(float value)
  { mCounter = (int)value; }

  float increment_counter()
  { return (float)++mCounter; }
  
private:
  int mCounter;
};

extern "C" void init()
{
  Node::declare<Counter>("Counter");
}