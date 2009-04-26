#include "rubyk.h"

class Metro : public Node
{
public:
  Metro() : tempo_(120), run_(false) {}
  
  const Value init() {
    run_ = true;
    bang(gNilValue); // start loop
    return gTrueValue;
  }
  
  // [1] restart metronome / set tempo
  const Value tempo(const Value &val) {
    if (val.is_real()) change_tempo(val.r);
    
    return Value(tempo_);
  }
  
  // [2] stop metronome
  const Value start_stop(const Value &val) {
    if (val.is_real()) {
      if (val.r == 0.0) {
        // stop
        remove_my_events();
        run_ = false;
      } else {
        // start
        if (!run_) bang_me_in(ONE_MINUTE / tempo_);
        run_ = true;
      }
    }
    
    return Value(run_);
  }
  
  virtual void inspect(Value *hash) const {
    hash->set("tempo", tempo_);
    hash->set("run", run_);
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
      tempo_ = tempo;
      if ((ONE_MINUTE / tempo_) < (WORKER_SLEEP_MS + 1)) {
        tempo_ = ONE_MINUTE / (WORKER_SLEEP_MS * 1.5);
      }
      remove_my_events();
      if (run_) bang_me_in(ONE_MINUTE / tempo_);
    }
  }
  
  Real tempo_;
  bool run_;
};

extern "C" void init(Planet &planet) {
  CLASS( Metro, "Metronome that sends bangs at regular intervals.", "tempo: [initial tempo]")
  METHOD(Metro, tempo,RealIO("bpm", "Restart metronome | set tempo value."))
  METHOD(Metro, start_stop, RealIO("1,0", "Start/stop metronome."))
  OUTLET(Metro, bang, BangIO("Regular bangs."))
}