#include "node.h"

class Add : public Node
{
public:
  
  bool init (const Params& p)
  {
    mValue1 = p.get("value1", 0);
    mValue2 = p.get("value2", 0);
    
    make_inlet<Add,&Add::set_value1>();
    make_inlet<Add,&Add::set_value2>();
    make_outlet<Add,&Add::sum>();
    
    return true;
  }

  virtual const char * spy() 
  { 
    sprintf(mSpy, "%i",(int)( mValue1 + mValue2 ));
    return mSpy;
  }
  
  void set_value1(float value)
  { mValue1 = value; }
  
  void set_value2(float value)
  { mValue2 = value; }

  float sum()
  { return mValue1 + mValue2; }
  
private:
  float mValue1;
  float mValue2;
};

extern "C" void init()
{
  Node::declare<Add>("Add");
}