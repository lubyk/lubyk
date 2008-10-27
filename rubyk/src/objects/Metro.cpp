#include "class.h"

class Metro : public Node
{
public:
  bool set(const Params& p)
  {
    mTempo = p.val("tempo", 120, true); // 120bmp default
    
    remove_my_events();
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
      send(gBangSignal);
    }
  }
  
  virtual void spy()
  { bprint(mSpy, mSpySize,"%.2f", mTempo );  }
  
  
private:
  double mTempo;
};

extern "C" void init()
{
  CLASS( Metro)
  OUTLET(Metro,bang)
}