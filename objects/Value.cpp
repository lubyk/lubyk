#include "class.h"

class Value : public Node
{
public:
  
  bool init (const Params& p)
  {
    double value;
    if (p.size() > 1) {
      for(int i=0; i < p.size(); i++) {
        p.get(&value, i);
        mBuffer.append(value);
      }
      mS.set(mBuffer);
    } else if (p.get(&value, "value", true)) {
      mS.set(value);
    } else {
      mS.set_nil();
    }
    
    return true;
  }
  
  // inlet 1
  void bang(const Signal& sig)
  { 
    double value;
    if (sig.type == ArraySignal) {
      // copy
      if(mBuffer.set(sig)) {
        mS.type = ArraySignal;
        mS.array.value = mBuffer.data;
        mS.array.size  = mBuffer.size;
      }
    } else {
      mS = sig;
    } 
    send(mS);
  }

  virtual void spy() 
  { std::ostringstream os(std::ostringstream::out);
    os << mS;
    bprint(mSpy, mSpySize,"%s", os.str().c_str() );
  }
  
private:
  Buf<double> mBuffer;
};

extern "C" void init()
{
  CLASS (Value)
  OUTLET(Value, value)
}