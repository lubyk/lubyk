#include "rubyk.h"

class Metro : public Node
{
public:
  bool set(const Value& p)
  {
    mTempo = p.val("tempo", 120, true); // 120bmp default
    
    remove_my_events();
    bang_me_in(0.0);
    return true;
  }
  
  // inlet 1
  void bang(const Value& val)
  {  
    if (val.type) { // bang_me_in class 'bang' with a NilValue, if it's not nil, it's a resync/set bang
      val.get(&mTempo);
      remove_my_events();
    }
    if (mTempo != 0) {
      bang_me_in(ONE_MINUTE / mTempo);
      send(gBangValue);
    }
  }
  
  virtual const Value inspect(const Value& val) 
  { bprint(mSpy, mSpySize,"%.2f", mTempo );  }
  
  void stop(const Value& val)
  {
    remove_my_events();
  }
  
  void start(const Value& val)
  {
    bang(sig);
  }
  
private:
  real_t mTempo;
};

extern "C" void init()
{
  CLASS( Metro)
  INLET(Metro,start)
  INLET(Metro,stop)
  OUTLET(Metro,bang)
}