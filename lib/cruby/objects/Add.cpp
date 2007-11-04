#include "class.h"

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
  { sig.get(&mValue1); }
  
  void set_value2(const Signal& sig)
  { sig.get(&mValue2); }

  void sum(Signal& sig)
  { sig.set(mValue1 + mValue2); }
  
private:
  float mValue1;
  float mValue2;
};

extern "C" void init()
{
  Class::declare<Add>("Add");
}