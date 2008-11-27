#include "class.h"

class ValueObj : public Node
{
public:
  
  bool set (const Value& p)
  {
    real_t value;
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
      mS.set_bang();
    }
    
    return true;
  }
  
  // inlet 1
  void bang(const Value& sig)
  { 
    set_value(sig);
    send(mS);
  }
  
  // inlet 2: set value
  void set_value(const Value& sig)
  {
    if (sig.type == MatrixValue) {
      // copy
      if(mMatrix.copy(sig)) {
        mS.set(mMatrix);
      }
    } else if (sig.type != BangValue) {
      mS = sig;
    }
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
  Class::declare<ValueObj>("Value");
  Class::find("Value")->add_inlet<ValueObj, &ValueObj::set_value>("set_value");
  OUTLET(Value, value)
}