#include "class.h"

class Bang : public Node
{
public:
  bool set (const Params& p)
  {
    return true;
  }
  
  // inlet 1
  void bang(const Signal& sig)
  {
    send(4,gBangSignal);
    send(3,gBangSignal);
    send(2,gBangSignal);
    send(1,gBangSignal);
  }
  
  virtual void spy()
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