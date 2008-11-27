#include "class.h"

class Print : public Node
{
public:
  
  bool set (const Value& p)
  {
    return true;
  }
  
  // inlet 1
  void bang(const Value& sig)
  { 
    *mOutput << sig << std::endl;
  }
  
  virtual void spy()
  { 
    bprint(mSpy, mSpySize,"");
  }
  
};

extern "C" void init()
{
  CLASS(Print)
}