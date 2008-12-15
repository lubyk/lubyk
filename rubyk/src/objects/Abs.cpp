#include "rubyk.h"

class Abs : public Node
{
public:
  bool set(const Value& p)
  {
    return true;
  }
  
  // inlet 1
  void bang(const Value& val)
  {  
    real_t d;
    int i;
    if (val.type == MatrixValue) {
      mBuffer.copy(sig);
      for(size_t i = 0; i < mBuffer.size(); i++)
        mBuffer.data[i] = mBuffer.data[i] < 0 ? -mBuffer.data[i] : mBuffer.data[i];
      send(mBuffer);
    } else if (val.type == IntegerValue) {
      val.get(&i);
      i < 0 ? send(-i) : send(i);
    } else if (val.type == DoubleValue) {
      val.get(&d);
      d < 0 ? send(-d) : send(d);
    } else {
      // pass through
      send(sig);
    }
  }
  
  virtual const Value inspect(const Value& val) 
  { bprint(mSpy, mSpySize, "");  }
  
private:
  Matrix mBuffer;
};

extern "C" void init()
{
  CLASS(Abs)
  OUTLET(Abs,absolute)
}