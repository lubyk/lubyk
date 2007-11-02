#include "node.h"
#include <cstdio>

class Metro : public Node
{
public:
  bool init(const Params& p)
  {
    mInterval = p.get("metro", 0.5); // 120bmp default
    make_inlet<Metro,&Metro::set_tempo>();
    make_outlet<Metro,&Metro::tic>();
    tic();
    return true;
  }

  void set_tempo(float value)
  { if (value) mInterval = 1.0/value; }
  
  const char * spy()
  { sprintf(mSpy, "%2f", mInterval); return mSpy; }
  
  float tic()
  {
    bang_me_in(mInterval);
    return 1.0;
  }
  
private:
  float mInterval;
};

extern "C" void init()
{
  Node::declare<Metro>("Metro");
}