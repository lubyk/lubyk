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

  virtual void spy() 
  { bprint(mSpy, mSpySize,"%.2f", mValue );  }
  
  void set_value(const Signal& sig)
  { sig.get(&mValue); }

  void value(Signal& sig)
  { sig.set(mValue); }
  
private:
  double mValue;
};

extern "C" void init()
{
  CLASS (Value)
  INLET (Value, set_value)
  OUTLET(Value, value)
}