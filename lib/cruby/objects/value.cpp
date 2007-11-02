#include "node.h"

class Value : public Node
{
public:
  
  bool init (const Params& p)
  {
    mValue = p.get("value", 0);
    
    make_inlet<Value,&Value::set_value>();
    make_outlet<Value,&Value::value>();
    
    return true;
  }

  virtual void spy() 
  { spy_print("%.2f", mValue );  }
  
  void set_value(const Signal& sig)
  { SET_FLOAT(mValue, sig) }

  void value(Signal& sig)
  { SEND_FLOAT(sig, mValue) }
  
private:
  float mValue;
};

extern "C" void init()
{
  Node::declare<Value>("Value");
}