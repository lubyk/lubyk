#include "rubyk.h"

class Metro : public Node
{
public:
  
  bool init()
  {
    mTempo = 120;
    
    remove_my_events();
    bang_me_in(0); // start loop
    return true;
  }
  
  /** Set/get tempo. Set to 0 to stop. */
  const Value tempo_ (const Value &val)
  {
    Real old_tempo = mTempo.value();
    mTempo = val;
    if (mTempo < 0) mTempo = 0;
    
    if (old_tempo != mTempo) {
      // tempo changed
      remove_my_events();
      if (mTempo != 0) bang_me_in(ONE_MINUTE / mTempo);
    }
    
    return mTempo;
  }
  
  // [1] restart metronome / set tempo
  void bang(const Value &val)
  { 
    if (val.is_number()) {
      std::cout << val << " is_number" << std::endl;
      mTempo = val;
      remove_my_events();
      if (mTempo < 0) mTempo = 0;
    }
    
    if (mTempo != 0) {
      // TODO: optimize: reuse previous event (send it in Value& val as EventValue ?)
      bang_me_in(ONE_MINUTE / mTempo.value());
      send(gBangValue);
    }
  }
  
  // [2] stop metronome
  void stop(const Value &val)
  {
    remove_my_events();
  }
  
  virtual const Value inspect(const Value &val)  
  { 
    std::ostringstream oss; // <Counter:/v1 3 (+1)>
    oss << "<Metro:" << url() << " " << mTempo << ">";
    return String(oss.str());
  }
  
private:
  Number mTempo;
};

extern "C" void init(Worker& planet)
{
  CLASS(   Metro, "Metronome that sends bangs at regular intervals.")
  INLET(   Metro, bang, BangValue | NumberValue, "Restart metronome | set tempo value.")
  INLET(   Metro, stop, AnyValue, "Stop metronome.")
  OUTLET(  Metro, bang, BangValue, "Regular bangs.")
  ACCESSOR(Metro, tempo, "Set tempo.")
}