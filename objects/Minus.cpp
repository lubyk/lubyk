#include "class.h"

class Minus : public Node
{
public:
  bool set(const Params& p)
  {
    mValue = p.val("minus", 1.00, true);
    return true;
  }
  
  // inlet 1
  void bang(const Signal& sig)
  {  
    double d;
    if (sig.type == MatrixSignal) {
      mBuffer.copy(sig);
      mBuffer -= mValue;
      mS.set(mBuffer);
      send(mS);
    } else if (sig.get(&d)) {
      // single value
      d -= mValue;
      send(d);
    }
  }
  
  // inlet 2
  void set_minus(const Signal& sig)
  {
    sig.get(&mValue);
  }
  
  virtual void spy()
  { bprint(mSpy, mSpySize,"-%.2f", mValue );  }
  
  
private:
  Matrix mBuffer;
  double mValue;
};

extern "C" void init()
{
  CLASS( Minus)
  INLET( Minus, set_minus)
  OUTLET(Minus,bang)
}