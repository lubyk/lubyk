#include "class.h"

class Minus : public Node
{
public:
  bool init(const Params& p)
  {
    mValue = p.val("minus", 1.00, true);
    return true;
  }
  
  // inlet 1
  void bang(const Signal& sig)
  {  
    double d;
    if (sig.type == ArraySignal) {
      mBuf.set(sig);
      mBuf -= mValue;
      mS.set(mBuf);
      if (mDebug) *mOutput << mName << ": " << mS << std::endl;
      send(mS);
    } else if (sig.get(&d)) {
      // single value
      d -= mValue;
      if (mDebug) *mOutput << mName << ": " << d << std::endl;
      send(d);
    }
  }
  
  void set_minus(const Signal& sig)
  {
    sig.get(&mValue);
  }
  
  virtual void spy()
  { bprint(mSpy, mSpySize,"-%.2f", mValue );  }
  
  
private:
  Buf<double> mBuf;
  double mValue;
};

extern "C" void init()
{
  CLASS( Minus)
  INLET( Minus, set_minus)
  OUTLET(Minus,bang)
}