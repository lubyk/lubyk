#include "class.h"

class Foo : public Node
{
  // default inlet
  void bang (const Signal& sig)
  {
    Matrix m = sig; // light copy (shares memory with "sig")
    m *= mMult;     // real copy (with multiplication)
    send(1,m);      // "m" is automatically deallocated
  }

  // get mMult parameter
  const Signal& mult ()
  {
    return Signal(mMult);
  }

  // inlet 2: set mMult (also used as param)
  void mult (const Signal& sig)
  {
    mMult = sig;
  }
  
private:
  double mMult;
}

extern "C" void init()
{
  CLASS (Foo)
  INLET (Foo,mult)
  OUTLET(Foo,mult)
  ACCESSOR(Foo,mult)
}