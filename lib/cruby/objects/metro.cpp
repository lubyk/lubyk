#include "node.h"

class Metro : public Node
{
public:
  bool init(const Params& p)
  {
    mSpeed = p.get("speed", 0.5); // 120bmp default
    make_inlet<Metro,&Metro::set_tempo>("set tempo");
    make_outlet<Metro,&Metro::tic>("send a tic");
    return true;
  }

  void set_tempo(float value)
  { if (value) mSpeed = 1.0/value; }
  
  float tic()
  {
    bang_me_at(gLogicalTime + mSpeed);
    return BANG;
  }
private:
  float mSpeed;
}