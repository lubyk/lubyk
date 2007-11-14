#include "class.h"

class Value : public Node
{
public:
  
  bool init (const Params& p)
  {
    // first try 'value', then default
    if (!p.get(&mValue, "value", true)) {
      mValue = 0.0;
    }
    
    return true;
  }
  
  // inlet 1
  void bang(const Signal& sig)
  { 
    sig.get(&mValue);
    send(mValue); 
  }

  virtual void spy() 
  { bprint(mSpy, mSpySize,"%.2f", mValue );  }
  
private:
  double mValue;
};

extern "C" void init()
{
  CLASS (Value)
  OUTLET(Value, value)
}