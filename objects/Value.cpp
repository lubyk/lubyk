#include "class.h"

class Value : public Node
{
public:
  
  bool set (const Params& p)
  {
    double value;
    if (p.size() > 1) {
      if (!mMatrix.set_sizes(1, p.size())) return false;
      for(size_t i=0; i < p.size(); i++) {
        p.get(&value, i);
        mMatrix.data[i] = value;
      }
      mS.set(mMatrix);
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
    if (sig.type == MatrixSignal) {
      // copy
      if(mMatrix.copy(sig)) {
        mS.set(mMatrix);
      }
    } else if (sig.type) {
      mS = sig;
    }
    mS.set_meta(H("foo"), std::string("Bar"));
    std::string s;
    
    mS.get_meta(&s, H("foo"));
    send(mS);
  }

  virtual void spy() 
  { std::ostringstream os(std::ostringstream::out);
    os << mS;
    bprint(mSpy, mSpySize,"%s", os.str().c_str() );
  }
  
private:
  Matrix mMatrix;
};

extern "C" void init()
{
  CLASS (Value)
  OUTLET(Value, value)
}