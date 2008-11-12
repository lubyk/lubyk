#include "class.h"

class Add : public Node
{
public:
  void init ()
  {
    mValue1 = 0;
    mValue2 = 0;
  }
  
  void set_value1 (const Signal& sig)
  {
    sig.get(&mValue1);
  }
  
  void get_value1 (Signal& sig)
  {
    sig.set(mValue1)
  }
  
  void tempo (VALUE d)
  {
    mValue1 = d;
  }
  
  const VALUE tempo () const
  {
    return mValue1;
  }
  
  ATTR_ACCESSOR(mValue1)
  
  
  bool set (const Params& p)
  {
    p.get(&mValue1, "value1");
    p.get(&mValue2, "value2");   
    return true;
  }

  virtual void spy() 
  { bprint(mSpy, mSpySize,"%.2f", mValue1 + mValue2 );  }
  
  void bang(const Signal& sig)
  { 
    sig.get(&mValue1);
    send(mValue1 + mValue2);
  }
  
  void value2(const Signal& sig)
  { sig.get(&mValue2); }

  
private:
  double mValue1;
  double mValue2;
};

extern "C" void init()
{
  CLASS (Add)
  INLET (Add, value2)
  OUTLET(Add, sum)
}