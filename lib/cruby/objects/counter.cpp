#include "node.h"
/** This is a very basic object to see what an object definition could look like. */

class Counter : public Node
{
public:
  // params = hash, 'port:450, baud:3200, xyz:450, title:"home of the braave"'
  bool init (const Params& p)
  {
    mCounter = p.get("counter", 0);
    mName    = p.get("name"   , std::string("no-name"));
    
    make_inlet<Counter,&Counter::set_counter>();
    make_outlet<Counter,&Counter::increment_counter>();
    
    return true;
  }

  virtual const char * spy() 
  { 
    sprintf(mSpy, "%s: %i", mName.c_str(), mCounter);
    return mSpy;
  }
  
  void set_counter(float value)
  { mCounter = (int)value; }

  float increment_counter()
  { return (float)++mCounter; }
  
private:
  int mCounter;
  std::string mName;
};

extern "C" void init()
{
  Node::declare<Counter>("counter");
}