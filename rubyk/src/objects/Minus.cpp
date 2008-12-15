#include "rubyk.h"

class Minus : public Node
{
public:
  bool set(const Value& p)
  {
    mValue = p.val("minus", 1.00, true);
    return true;
  }
  
  // inlet 1
  void bang(const Value& val)
  {  
    real_t d;
    if (val.type == MatrixValue) {
      mBuffer.copy(sig);
      mBuffer -= mValue;
      send(mBuffer);
    } else if (val.get(&d)) {
      // single value
      d -= mValue;
      send(d);
    } else {
      // pass through
      send(sig);
    }
  }
  
  // inlet 2
  void set_minus(const Value& val)
  {
    val.get(&mValue);
  }
  
  virtual const Value inspect(const Value& val) 
  { bprint(mSpy, mSpySize,"-%.2f", mValue );  }
  
  
private:
  Matrix mBuffer;
  real_t mValue;
};

extern "C" void init()
{
  CLASS( Minus)
  INLET( Minus, set_minus)
  OUTLET(Minus,bang)
}