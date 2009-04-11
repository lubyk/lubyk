#include "rubyk.h"

class Bang : public Node
{
public:
  bool set (const Value &p)
  {
    return true;
  }
  
  // inlet 1
  void bang(const Value &val)
  {
    send(4,gBangValue);
    send(3,gBangValue);
    send(2,gBangValue);
    send(1,gBangValue);
  }
  
  virtual const Value inspect(const Value &val) 
  { bprint(mSpy, mSpySize,"");  }
  
};

extern "C" void init()
{
  CLASS( Bang)
  OUTLET(Bang,bang)
  OUTLET(Bang,bang2)
  OUTLET(Bang,bang3)
  OUTLET(Bang,bang4)
}