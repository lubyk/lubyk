#include "node.h"

class Add : public Node
{
public:
  
  bool init (const Params& p)
  {
    mValue1 = p.get("value1", 0);
    mValue2 = p.get("value2", 0);
    
    make_inlet<Add,&Add::set_value1>();
    make_inlet<Add,&Add::set_value2>();
    make_outlet<Add,&Add::sum>();
    
    return true;
  }

  virtual void spy() 
  { spy_print("%.2f", mValue1 + mValue2 );  }
  
  void set_value1(const Signal& sig)
  { SET_FLOAT(mValue1, sig) }
  
  void set_value2(const Signal& sig)
  { SET_FLOAT(mValue2, sig) }

  void sum(Signal& sig)
  { SEND_FLOAT(sig, mValue1 + mValue2) }
  
private:
  float mValue1;
  float mValue2;
};

extern "C" void init()
{
  Node::declare<Add>("Add");
}