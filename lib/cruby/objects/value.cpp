#include "node.h"

class Value : public Node
{
public:
  
  bool init (Params& p)
  {
    mValue = p.get("value", 0);
    
    make_inlet<Value,&Value::set_value>();
    make_outlet<Value,&Value::value>();
    
    return true;
  }

  virtual const char * spy() 
  { 
    sprintf(mSpy, "%i", (int)mValue);
    return mSpy;
  }
  
  void set_value(float value)
  { mValue = value; }

  float value()
  { return mValue; }
  
private:
  float mValue;
};

extern "C" void init()
{
  Node::declare<Value>("value");
}