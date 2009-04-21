#include "rubyk.h"

class Metro : public Node
{
public:
  
  bool init() {
    tempo_ = 120;
    
    remove_my_events();
    bang_me_in(0); // start loop
    run_ = true;
    return true;
  }
  
  // [1] restart metronome / set tempo
  const Value tempo(const Value &val) {
    Real old_tempo = tempo_;
    if (val.is_real()) change_tempo(val.r);
    
    return Value(tempo_);
  }
  
  // [2] stop metronome
  void stop(const Value &val) {
    remove_my_events();
    run_ = false;
  }
  
  virtual void inspect(Value *hash)  {
    hash.set("tempo", tempo_);
    hash.set("run", run_);
  }
  
  // internal use only (looped call)
  void bang(const Value &val) {
    if (run_) {
      // TODO: optimize: reuse previous event (send it in Value& val as EventValue ?)
      bang_me_in(ONE_MINUTE / tempo_);
      send(gNilValue);
    }
  }
  
private:
  void change_tempo(Real tempo) {
    if (tempo <= 0) {
      run_ = false;
    } else if (tempo != tempo_) {
      // tempo changed
      remove_my_events();
      if (run_) bang_me_in(ONE_MINUTE / tempo_);
    }
  }
  
  Real tempo_;
  bool run_;
};

extern "C" void init(Worker& planet)
{
  CLASS( Metro, RealIO("bpm", "Metronome that sends bangs at regular intervals."))
  METHOD(Metro, tempo,RealIO("bpm", "Restart metronome | set tempo value."))
  INLET( Metro, stop, AnyIO("Stop metronome."))
  OUTLET(Metro, bang, BangIO("Regular bangs."))
}