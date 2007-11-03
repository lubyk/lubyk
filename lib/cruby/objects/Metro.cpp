#include "class.h"
#include <cstdio>

class Metro : public Node
{
public:
  bool init(const Params& p)
  {
    mTempo = p.get("metro", 0.5); // 120bmp default
    make_inlet<Metro,&Metro::set_tempo>();
    make_outlet<Metro,&Metro::tic>();
    bang_me_in(0.0);
    return true;
  }

  void set_tempo(const Signal& sig)
  { SET_FLOAT(mTempo, sig) }
  
  virtual void spy()
  { spy_print("%.2f", mTempo );  }
  
  void tic(Signal& sig)
  {
    if (mTempo != 0) {
      bang_me_in(60000 / mTempo);
      SEND_BANG(sig);
    }
  }
  
private:
  float mTempo;
};

extern "C" void init()
{
  Class::declare<Metro>("Metro");
}