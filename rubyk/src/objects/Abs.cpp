#include "class.h"

class Abs : public Node
{
public:
  bool set(const Value& p)
  {
    return true;
  }
  
  // inlet 1
  void bang(const Value& sig)
  {  
    real_t d;
    int i;
    if (sig.type == MatrixValue) {
      mBuffer.copy(sig);
      for(size_t i = 0; i < mBuffer.size(); i++)
        mBuffer.data[i] = mBuffer.data[i] < 0 ? -mBuffer.data[i] : mBuffer.data[i];
      send(mBuffer);
    } else if (sig.type == IntegerValue) {
      sig.get(&i);
      i < 0 ? send(-i) : send(i);
    } else if (sig.type == DoubleValue) {
      sig.get(&d);
      d < 0 ? send(-d) : send(d);
    } else {
      // pass through
      send(sig);
    }
  }
  
  virtual void spy()
  { bprint(mSpy, mSpySize, "");  }
  
private:
  Matrix mBuffer;
};

extern "C" void init()
{
  CLASS(Abs)
  OUTLET(Abs,absolute)
}