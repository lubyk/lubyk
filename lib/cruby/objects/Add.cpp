#include "class.h"

class Add : public Node
{
public:
  
  bool init (const Params& p)
  {
    mValue1 = p.val("value1", 0);
    mValue2 = p.val("value2", 0);
    
    return true;
  }

  virtual void spy() 
  { bprint(mSpy, mSpySize,"%.2f", mValue1 + mValue2 );  }
  
  void value1(const Signal& sig)
  { sig.get(&mValue1); }
  
  void value2(const Signal& sig)
  { sig.get(&mValue2); }

  void bang()
  {
    send(mValue1 + mValue2);
  }
  
private:
  double mValue1;
  double mValue2;
};

extern "C" void init()
{
  CLASS (Add)
  INLET (Add, value1)
  INLET (Add, value2)
  OUTLET(Add, sum)
}