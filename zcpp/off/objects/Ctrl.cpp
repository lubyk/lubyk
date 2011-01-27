#include "lubyk.h"
#include "midi_message.h"
#include <sstream>

#define CTRL_MIN_TIME_STEP 100  // [ms]
class Ctrl : public Node
{
public:
  bool init(const Value &p)
  {
    mMessage.mType = CtrlChange;
    mMessage.set_ctrl(   1    );
    mMessage.set_value(  64   );
    mSlope           =   0; // direct
    mMessage.set_channel(1);
    
    return true;
  }
  
  bool set(const Value &p)
  {  
    mMessage.set_ctrl(      p.val("ctrl",   mMessage.ctrl()    ));
    mMessage.set_value(     p.val("value",  mMessage.velocity()));
    mSlope             =    p.val("slope",  mSlope   );
    mMessage.set_channel(   p.val("channel",mMessage.channel() ));
    return true;
  }
  
  // inlet 1 (set ctrl value)
  void bang(const Value &val)
  {
    Real i;
    if (val.type == MidiValue && val.midi_ptr.value->mType == CtrlChange) {
      mMessage = *(val.midi_ptr.value);
      send(mMessage);
    } else if (val.get(&i)) {
      if (mSlope) {
        mTarget = i;
        remove_my_events(); // restart triggers
        move();
      } else {
        mMessage.set_value(i);
        send(mMessage);
      }
    } else {
      send(mMessage);
    }
  }
  
  // inlet 2
  void set_ctrl(const Value &val)
  {
    int n;
    if (val.get(&n)) mMessage.set_note(n);
  }
  
  // inlet 3
  void set_slope(const Value &val)
  {
    val.get(&mSlope);
  }
  
  // inlet 4
  void set_channel(const Value &val)
  {
    int i;
    if (val.get(&i)) mMessage.set_channel(i);
  }
  
  void move ()
  {
    do_move(NULL);
  }
  
  void clear()
  { remove_my_events(); }
  
  virtual const Value inspect(const Value &val) 
  { 
    std::ostringstream oss(std::ostringstream::out);
    oss << mMessage;
    bprint(mSpy, mSpySize,"%s", oss.str().c_str());
  }
  
private:
  
  void do_move(void * data)
  {
    if (mMessage.value() == mTarget || !mSlope) return;
    int delta = mTarget - mMessage.value();
    time_t duration = delta < 0 ? -1000 * delta / mSlope : 1000 * delta / mSlope; // [ms];
    if (duration < CTRL_MIN_TIME_STEP) {
      // jump to value
      mMessage.set_value(mTarget);
      send(mMessage);
      return;
    }
    
    time_t next = 1000 / mSlope; // time to move 1 step
    next = next < CTRL_MIN_TIME_STEP ? CTRL_MIN_TIME_STEP : next;
    if (delta > 0) // move up slope * CTRL_MIN_TIME_STEP [ms]
      mMessage.set_value(mMessage.value() + next * mSlope / 1000);
    else   
      mMessage.set_value(mMessage.value() - next * mSlope / 1000);
      // moving down
    send(mMessage);
    REGISTER_EVENT(Ctrl, do_move, next, NULL);
  }
  
  /* data */
  MidiMessage  mMessage;
  time_t mSlope;
  unsigned int mTarget;
};

extern "C" void init(Planet &planet) {
  CLASS (Ctrl)
  INLET (Ctrl, set_ctrl)
  INLET (Ctrl, set_slope)
  INLET (Ctrl, set_channel)
  OUTLET(Ctrl, send)
  METHOD(Ctrl, move)
  METHOD(Ctrl, clear)
}