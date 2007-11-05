#include "class.h"

class Metro : public Node
{
public:
  bool init(const Params& p)
  {
    mTempo = p.val("metro", 0.5); // 120bmp default
    
    bang_me_in(0.0);
    return true;
  }

  void set_tempo(const Signal& sig)
  { sig.get(&mTempo); }
  
  virtual void spy()
  { spy_print("%.2f", mTempo );  }
  
  void tic(Signal& sig)
  {
    if (mTempo != 0) {
      bang_me_in(60000 / mTempo);
      sig.set_bang();
    }
  }
  
private:
  float mTempo;
};

extern "C" void init()
{
  Class * klass = Class::declare<Metro>("Metro");
  
  klass->add_inlet<Metro,&Metro::set_tempo>( "set_tempo");
  klass->add_outlet<Metro,&Metro::tic>(      "tic");
}