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

  void set(const Signal& sig)
  { 
    remove_my_events();
    sig.get(&mTempo);
    bang();
  }
  
  virtual void spy()
  { bprint(mSpy, mSpySize,"%.2f", mTempo );  }
  
  void bang()
  {
    if (mTempo != 0) {
      bang_me_in(ONE_MINUTE / mTempo);
      send(BangSignal);
    }
  }
  
private:
  float mTempo;
};

extern "C" void init()
{
  CLASS( Metro)
  INLET( Metro,set)
  OUTLET(Metro,tic)
}