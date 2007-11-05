#include "class.h"

class Value : public Node
{
public:
  
  bool init (const Params& p)
  {
    // first try 'value', then default
    if (!p.get(&mValue, "value", true)) {
      mValue = 0.0;
    }
    
    return true;
  }

  virtual void spy() 
  { spy_print("%.2f", mValue );  }
  
  void set_value(const Signal& sig)
  { sig.get(&mValue); }

  void value(Signal& sig)
  { sig.set(mValue); }
  
private:
  float mValue;
};

extern "C" void init()
{
  Class * klass = Class::declare<Value>("Value");
  
  klass->add_inlet<Value,&Value::set_value>( "set_value" );
  klass->add_outlet<Value,&Value::value>(    "value"     );
  
}