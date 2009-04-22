#include "rubyk.h"

class Minus : public Node
{
public:
  bool set(const Value &p)
  {
    value_ = p.val("minus", 1.00, true);
    return true;
  }
  
  // inlet 1
  void bang(const Value &val)
  {  
    Real d;
    if (val.type == MatrixValue) {
      mBuffer.copy(sig);
      mBuffer -= value_;
      send(mBuffer);
    } else if (val.get(&d)) {
      // single value
      d -= value_;
      send(d);
    } else {
      // pass through
      send(sig);
    }
  }
  
  // inlet 2
  void set_minus(const Value &val)
  {
    val.get(&value_);
  }
  
  virtual const Value inspect(const Value &val) 
  { bprint(mSpy, mSpySize,"-%.2f", value_ );  }
  
  
private:
  Matrix mBuffer;
  Real value_;
};

extern "C" void init(Planet &planet) {
  CLASS( Minus)
  INLET( Minus, set_minus)
  OUTLET(Minus,bang)
}