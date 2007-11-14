#include "class.h"

class Metro : public Node
{
public:
  bool init(const Params& p)
  {
    mTempo = p.val("metro", 120); // 120bmp default
    
    bang_me_in(0.0);
    return true;
  }
  
  // inlet 1
  void bang(const Signal& sig)
  {  
    if (sig.type) { // bang_me_in class 'bang' with a NilSignal, if it's not nil, it's a resync/set bang
      sig.get(&mTempo);
      remove_my_events();
    }
    if (mTempo != 0) {
      bang_me_in(ONE_MINUTE / mTempo);
      send(BangSignal);
    }
  }
  
  virtual void spy()
  { bprint(mSpy, mSpySize,"%.2f", mTempo );  }
  
  
private:
  float mTempo;
};

extern "C" void init()
{
  CLASS( Metro)
  OUTLET(Metro,bang)
}